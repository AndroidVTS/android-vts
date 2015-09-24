
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

enum {
    // Media errors
    MEDIA_ERROR_BASE        = -1000,

    ERROR_ALREADY_CONNECTED = MEDIA_ERROR_BASE,
    ERROR_NOT_CONNECTED     = MEDIA_ERROR_BASE - 1,
    ERROR_UNKNOWN_HOST      = MEDIA_ERROR_BASE - 2,
    ERROR_CANNOT_CONNECT    = MEDIA_ERROR_BASE - 3,
    ERROR_IO                = MEDIA_ERROR_BASE - 4,
    ERROR_CONNECTION_LOST   = MEDIA_ERROR_BASE - 5,
    ERROR_MALFORMED         = MEDIA_ERROR_BASE - 7 
};


/*

MPEG4Extractor::MPEG4Extractor(const sp<DataSource> &source)
    : mMoofOffset(0),
      mDataSource(source),
      mInitCheck(NO_INIT),
      mHasVideo(false),
      mHeaderTimescale(0),
      mFirstTrack(NULL),
      mLastTrack(NULL),
      mFileMetaData(new MetaData),
      mFirstSINF(NULL),
      mIsDrm(false) {
}
*/


void *(*mpeg4ExtractorConstructor)(void *object, void * dataSource);

/*
status_t MPEG4Extractor::parseITunesMetaData(off64_t offset, size_t size) {
*/
status_t (*parseItunesMeta)(void *object, off64_t offset, size_t size);





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


int yolo(){
  return 1337;
}


static void die(const char *msg)
{
        printf("%s\n", msg);
        exit(-1);
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

int checkItunesMetaIsVulnerable() {
  void * libstagefright = dlopen("libstagefright.so",0);
  if(!libstagefright){
    die("[-] dlopen failed");
  }

  parseItunesMeta           = resolveSymbol(libstagefright, "_ZN7android14MPEG4Extractor19parseITunesMetaDataExj");

  const int numOfItems  = 1000;
  size_t mpeg4DataSource[numOfItems];
  size_t mpeg4ExtractorObj[numOfItems];
  size_t *p = (size_t*)mpeg4DataSource;
  int i;
  for(i = 0; i < numOfItems; i++){
    mpeg4DataSource[i] = (size_t) &yolo;
    mpeg4ExtractorObj[i] = (size_t) &p;
  }

  errno = 0;
  int ret = parseItunesMeta(mpeg4ExtractorObj, 0, SIZE_MAX);
  switch(ret){
    case ERROR_MALFORMED:
          return 0;
    case ERROR_IO:
          return 1;
    default:
          return -1;
  }

  return 0;
}



JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_framework_media_Stagefright_checkItunesMeta(JNIEnv *env, jobject obj){
  return 0;
}

void sig_handler(int signo)
{
  printf("Boom goes the dynamite\n");
  fflush(stdout);
  exit(-1);
}

int main(void){
  printf("ello, mate\n");
  printf("checkItunesMeta %d\n", checkItunesMetaIsVulnerable());
  return 1;
}


int main1(int argc, char *argv[]){
   if(argc < 2){
     printf("Usage %s <media_file>", argv[0]);
     return -1;
   }

   struct sigaction action;
   bzero(&action, sizeof(struct sigaction));

   action.sa_handler = sig_handler;
   action.sa_mask = SA_RESTART;

   sigaction(SIGSEGV, &action, NULL);
   sigaction(SIGABRT, &action, NULL);
   sigaction(SIGBUS, &action, NULL);
   sigaction(SIGFPE, &action, NULL);
   sigaction(SIGILL, &action, NULL);
   sigaction(SIGPIPE, &action, NULL);
   sigaction(SIGTRAP, &action, NULL);

   printf("Running stagefright detector!\n");

   char * media_file = argv[1];
   process_media_file(media_file);

   return 0;
}

