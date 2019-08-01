#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <android/log.h>

/*
	When user loads a third-party ttf font:
	in frameworks/minikin/CmapCoverage.cpp::getCoverageFormat12
	if nGroups >= 0xfffffff0 / kGroupSize, then the ttf font will
	cause continuous rebooting
*/

int checkIsVulnerable(){
	FILE *file;
	long size;
	uint8_t *buffer;
	int result;
	

	file = fopen("/system/lib/libminikin.so", "rb");

	if(file == NULL){
		// Before 5.0, libminikin didn't exist
		// Hence, not vulnerable
		result = 0;
		goto done;	
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	buffer = (uint8_t *)malloc(sizeof(char)*size);

	fread(buffer, 1, size, file);

	/*
	 kGroupSize = 12
	 const size_t kMaxNGroups = 0xfffffff0 / kGroupSize ~ 0x15555553 (how it appears in the assembly)
	*/

	// Check if this byte-sequence is in libminikin.so file
	uint8_t needle[4] = {0x53, 0x55, 0x55, 0x15};

	uint8_t *p = memmem(buffer, size, needle, 4);

	// If the byte-sequence is present, that means the bug has been patched
	if(p)
		result = 0;
	else	// Otherwise, there is no check for nGroups <=> vulnerable
		result = 1;

	fclose(file);
	free(buffer);

done:
	return result;
}

JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_framework_graphics_CVE_12016_10808_checkCVE20160808(JNIEnv *env, jobject obj){
	return checkIsVulnerable();
}

int main(void){
	return checkIsVulnerable();
}
