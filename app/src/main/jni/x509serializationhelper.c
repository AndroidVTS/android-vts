#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>

int myPositiveInt = 0x1337;
JNIEXPORT jlong JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_framework_serialization_OpenSSLTransientBug_getPositiveIntLocation(JNIEnv *env, jobject obj){
  myPositiveInt = 0x1337;
  return (unsigned long) &myPositiveInt;
}
