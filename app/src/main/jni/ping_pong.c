#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <linux/netlink.h>
#include <linux/if.h>
#include <linux/filter.h>
#include <linux/if_pppox.h>
#include <linux/sock_diag.h>
#include <linux/inet_diag.h>
#include <linux/unix_diag.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>

#define MMAP_BASE 0x200000
#define LIST_POISON 0x200200
#define MMAP_SIZE 0x200000

// Thanks, @beaups!
int checkIsVulnerable()
{
    void * magic = mmap((void *) MMAP_BASE, MMAP_SIZE,
       PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED | MAP_ANONYMOUS,
       -1, 0);
    memset(magic, 0, MMAP_SIZE);
    *((long *)(LIST_POISON)) = 0xfefefefe;
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    connect(sock, (const struct sockaddr *) &sa, sizeof(sa));
    sa.sin_family = AF_UNSPEC;
    connect(sock, (const struct sockaddr *) &sa, sizeof(sa));
    close(sock);
    if (*((long *)(LIST_POISON)) != 0xfefefefe){
       printf("Device is vulnerable\n");
       return 1;
    }else{
      printf("Device is not vulnerable\n");
      return 0;
    }
}

JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_kernel_CVE_12015_13636_checkPingPong(JNIEnv *env, jobject obj){
   return checkIsVulnerable();
}


int main(void){

  int r =  checkIsVulnerable();

  if(r == 0){
    printf("Device is not vulnerable\n");
  }else if(r == 1){
    printf("Device is vulnerable\n");
  }else{
     printf("There was an error running tests\n");
  }

  return 0;
}
