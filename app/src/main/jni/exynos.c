#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MMAP_SIZE   0x1000000
#define MMAP_OFFSET 0x40000000

int checkIsVulnerable()
{
	int fd;
	void *mem;
	int result;

	/* open the door */
	fd = open("/dev/exynos-mem", O_RDWR);
	if (fd == -1) {
		// patched|can't open device
		result = 0;
		goto done;
	}

	/* attempt to mmap the device fd */
	mem = mmap(NULL, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, MMAP_OFFSET);
	if (mem == MAP_FAILED) {
		// patched|can't mmap device
		result = 0;
		goto close;
	}

	// vulnerable|mmap succeeded
	result = 1;

close:
	close(fd);

done:
	return result;
}

JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_kernel_CVE_12012_16422_checkExynos(JNIEnv *env, jobject obj){
   return checkIsVulnerable();
}

int main(void){
  return checkIsVulnerable();
}
