
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>


/*
 * Class:     fuzion24_device_vulnerability_vulnerabilities_framework_serialization_OpenSSLTransientBug
 * Method:    getPositiveIntLocation
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_framework_serialization_OpenSSLTransientBug_getPositiveIntLocation
  (JNIEnv * env, jobject obj){
   //The bug will end up decrementing this value
   //Let's reset it in case the test gets ran 0x1337 times
//   somePositiveInt = 0x1337;
 //  return (long) &somePositiveInt;
  return 0l;
}
