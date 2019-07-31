#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <android/log.h>

int checkIsVulnerable(){
	FILE *file;
	long size;
	uint8_t *buffer;
	int result;
	

	file = fopen("/system/lib/libminikin.so", "rb");

	if(file == NULL){
		result = 0;
		goto done;	
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	buffer = (uint8_t *)malloc(sizeof(char)*size);

	fread(buffer, 1, size, file);
	uint8_t needle[4] = {0x53, 0x55, 0x55, 0x15};

	uint8_t *p = memmem(buffer, size, needle, 4);
	if(p)
		result = 0;
	else
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
