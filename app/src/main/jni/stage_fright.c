
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

typedef int32_t     status_t;


void *(*stageFrightConstructor)(void *object);

/*
status_t StagefrightMetadataRetriever::setDataSource(
        int fd, int64_t offset, int64_t length) {
*/

status_t (*setDataSource)(void *object, int fd, int64_t offset, long long length);

/*
const char *StagefrightMetadataRetriever::extractMetadata(int keyCode)
*/
status_t (*extractMetaData)(void *object, int keyCode);


static void die(const char *msg)
{
        perror(msg);
        exit(errno);
}


static void * resolveSymbol(void * lib, char * symbol){
    void * r;
    if ((r = (void *)dlsym(lib, symbol)) == NULL)
       die("[-] dlsym");
    return r;
}

int process_media_file(const char *media_file) {
  void * libstagefright = dlopen("libstagefright.so",0);
  if(!libstagefright){
    die("[-] dlopen failed");
  }

  stageFrightConstructor  = resolveSymbol(libstagefright, "_ZN7android28StagefrightMetadataRetrieverC1Ev");
  setDataSource           = resolveSymbol(libstagefright, "_ZN7android28StagefrightMetadataRetriever13setDataSourceEixx");
  extractMetaData         = resolveSymbol(libstagefright, "_ZN7android28StagefrightMetadataRetriever15extractMetadataEi");

  void * metaDataReceiverObject = malloc(0x100);
  if(!metaDataReceiverObject){
     die("[-] no memory for object");
  }

  stageFrightConstructor(metaDataReceiverObject);

  int testPOC = open(media_file, 0xa << 12);
  if(testPOC < 0){
   die("[-] failed opening file");
  }
  errno = 0;
  status_t ret = setDataSource(metaDataReceiverObject, testPOC, 0ull,0x7FFFFFFFFFFFFFFull);
  if(ret){
   printf("[-] setDataSource = 0x%x\n", ret);
   die("[-] setDataSource");
  }
  ret = extractMetaData(metaDataReceiverObject, 12);
  printf("ret value %d\n", ret);

  return 0;
}

JNIEXPORT jint JNICALL Java_com_device_vulnerability_vulnerabilities_framework_media_Stagefright_isVulnerable__Ljava_lang_String_2(JNIEnv *env, jobject obj, jstring media_file){
    const char * current_media_file;
    current_media_file = (*env)->GetStringUTFChars( env, media_file, NULL ) ;
   return process_media_file(current_media_file);
}

void sig_handler(int signo)
{
  printf("received Segfault\n");
  fflush(stdout);
  exit(0);
}



int main(int argc, char *argv[]){
   if(argc < 2){
     printf("Usage %s <media_file>", argv[0]);
     return -1;
   }

   struct sigaction action = {
           .sa_handler = sig_handler,
           .sa_mask = SA_RESTART
   };

   sigaction(SIGABRT, &action, NULL);
   sigaction(SIGBUS, &action, NULL);
   sigaction(SIGFPE, &action, NULL);
   sigaction(SIGILL, &action, NULL);
   sigaction(SIGPIPE, &action, NULL);
   sigaction(SIGSEGV, &action, NULL);
   sigaction(SIGTRAP, &action, NULL);


   char * media_file = argv[1];
   fflush(stdout);
   process_media_file(media_file);
   return 0;
}

