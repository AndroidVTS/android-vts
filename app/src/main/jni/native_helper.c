
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <fcntl.h>
#include<signal.h>

#include <jni.h>


int somePositiveInt = 0x1337;

JNIEXPORT jlong JNICALL Java_com_device_vulnerability_vulnerabilities_framework_serialization_OpenSSLTransientBug_getPositiveIntLocation(JNIEnv *env, jobject obj){
   //The bug will end up decrementing this value
   //Let's reset it in case the test gets ran 0x1337 times
   somePositiveInt = 0x1337;
   return (int) &somePositiveInt;
}

int main(){
return 0;
}
