
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

#define BIGBUF_SZ 0x10
int checklibUtils(){
   char bigbuf[BIGBUF_SZ];
   char strObj[0x1000];

   void *handle = dlopen( "libutils.so", RTLD_NOW );

   memset(strObj, 0, sizeof(strObj) );

   /* This corresponds to: 'android::String8::String8(char const*, unsigned int)' */
   void * ( *str8)(void * obj, char const * string, int len) = NULL;
   str8 = dlsym(handle, "_ZN7android7String8C2EPKcj");
   memset(bigbuf, 0xf0, BIGBUF_SZ);
   str8(strObj, (const char *)bigbuf, -1);
   char **ppstr = (char **)strObj;
   if ((*ppstr)[0] == 0xf0) {
       printf("Is Vulnerable\n");
   }else{
     printf("Is not vulnerable\n");
   }
}

int main(void){
  checklibUtils();
  return 1;
}


