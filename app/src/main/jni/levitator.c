#include <jni.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#define CONNECT_SERVICES 0xc01c670c
#define PVRSRV_MAX_BRIDGE_OUT_SIZE 0x1000
#define DUMP_SIZE PVRSRV_MAX_BRIDGE_OUT_SIZE + 4

typedef struct {
	uint32_t ui32BridgeID;
	uint32_t ui32Size;
	void *pvParamIn;
	uint32_t ui32InBufferSize;
	void *pvParamOut;
	uint32_t ui32OutBufferSize;
	void * hKernelServices;
} PVRSRV_BRIDGE_PACKAGE;

int checkIsVulnerable()
{
	int fd, ret;
	int result;
	char dump[DUMP_SIZE];
	PVRSRV_BRIDGE_PACKAGE pkg;

	fd = open("/dev/pvrsrvkm", O_RDWR);
	if (fd == -1) {
		// patched|can't open pvrsrvkm device
		result = 0;
		goto done;
	}

	memset(&pkg, 0, sizeof(pkg));

	pkg.ui32BridgeID = CONNECT_SERVICES;
	pkg.ui32Size = sizeof(pkg);
	pkg.ui32InBufferSize = 0;
	pkg.pvParamIn = NULL;
	pkg.ui32OutBufferSize = DUMP_SIZE;
	pkg.pvParamOut = dump;

	ret = ioctl(fd, 0, &pkg);
	if (ret == 0) {
		// vulnerable|leaked kernel memory
		result = 1;
		goto done;
	} else {
		// patched|can't leak kernel memory
		result = 0;
		goto done;
	}

done:
	return result;
}

JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_kernel_CVE_12011_11350_checkLevitator(JNIEnv *env, jobject obj){
   return checkIsVulnerable();
}

int main(void){
  return checkIsVulnerable();
}
