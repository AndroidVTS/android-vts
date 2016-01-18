#include <jni.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int
has_symbol(char *name)
{
	FILE *f;
	unsigned long addr;
	char dummy, sname[512];
	int ret = 0;
 
	f = fopen("/proc/kallsyms", "r");
	if (!f) {
		return 0;
	}
 
	while (ret != EOF) {
		ret = fscanf(f, "%p %c %s\n", (void **) &addr, &dummy, sname);
		if (ret == 0) {
			fscanf(f, "%s\n", sname);
			continue;
		}
		if (!strcmp(name, sname)) {
			return 1;
		}
	}

	return 0;
}

int checkIsVulnerable()
{
	int fd, ret, mem_rw, mem_release;
	int result;
	char buf[4];

	/* check to see if we have a mem_rw symbol */
	mem_rw = has_symbol("mem_rw");
	if (mem_rw) {
		// patched|found mem_rw ksym
		result = 0;
		goto done;
	}

	/* check to see if we have a mem_release symbol */
	mem_release = has_symbol("mem_release");
	if (mem_release) {
		// patched|found mem_release ksym
		result = 0;
		goto done;
	}

	/* probe to see is mem_write support is available */
	fd = open("/proc/self/mem", O_RDWR);
	if (fd == -1) {
		// patched|can't open /proc/pid/mem
		result = 0;
		goto done;
	}

	/* do a write(2) with count of 0 */
	ret = write(fd, buf, 0);
	if (ret == -1 && errno == EINVAL) {
		// patched|mem cannot be written to
		result = 0;
		goto done;
	} else {
		// vulnerable|mem can be written to
		result = 1;
		goto done;
	}

done:
	return result;
}

JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_kernel_CVE_12012_10056_checkMempodroid(JNIEnv *env, jobject obj){
   return checkIsVulnerable();
}

int main(void){
  return checkIsVulnerable();
}
