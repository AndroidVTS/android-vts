
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


void *(*graphicBufferConstructor)(void *object);


/*
status_t GraphicBuffer::unflatten(
        void const*& buffer, size_t& size, int const*& fds, size_t& count) {
*/
status_t (*graphicBufferUnflatten)(void *object, void const* buffer, size_t size, int const* fds, size_t count);


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

int graphicsBufferOverflowCheck() {
  void * libui = dlopen("libui.so",0);
  if(!libui){
    die("[-] dlopen failed");
  }

  graphicBufferConstructor = resolveSymbol(libui, "_ZN7android13GraphicBufferC2Ev");
  graphicBufferUnflatten = resolveSymbol(libui, "_ZN7android13GraphicBuffer9unflattenERPKvRjRPKiS4_");

  void * graphicBufferObject  = malloc(0x100);
  if(!graphicBufferObject){
     die("[-] no memory for object");
  }

  graphicBufferConstructor(graphicBufferObject);

  char buf[0x1000];

  const size_t maxNumber = UINT_MAX / sizeof(int);
  errno = 0;
  int numFds = 1;
  int count = 1;
  size_t size;
  status_t ret = graphicBufferUnflatten(graphicBufferObject, &buf, size, &numFds, count);
  if(ret){
    printf("[-] graphicsBufferUnflatten = 0x%x\n", ret);
    die("[-] graphicBufferUnflatten");
  }

  return 0;
}

void sig_handler(int signo)
{
  printf("Boom goes the dynamite\n");
  fflush(stdout);
  exit(-1);
}

int main(int argc, char *argv[]){
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

   printf("Running libui GraphicsBuffer detector!\n");

   graphicsBufferOverflowCheck();

   return 0;
}

