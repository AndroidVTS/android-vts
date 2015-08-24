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
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>

#define IP_MINTTL 21
#define IP_MULTICAST_ALL 49
#define IP_FREEBIND 15

int checkIsVulnerable()
{

    int fd = socket(AF_PPPOX, SOCK_RAW, PX_PROTO_OL2TP);

    if(fd < 0)
        return errno == 0 || errno == 1 ? -1 : errno;

    int optVal = 0x41;
    void *optvalP = &optVal;
    socklen_t sockLen = 20;

    int e1 = errno;

    int ret = setsockopt(fd, SOL_IP, IP_MULTICAST_ALL, optvalP, sockLen);
    ret = setsockopt(fd, SOL_IP, IP_MINTTL, optvalP, sockLen);
    ret = setsockopt(fd, SOL_IP, IP_FREEBIND , optvalP, sockLen);

    printf("Sockopt ret val %d\n", ret);

    if(ret == -1 && e1 == 14) return 0; //Not vulnerable
    if(ret == 0 && e1 == 0) return 1; //Vulnerable
    return errno == 0 || errno == 1 ? -1 : errno ; //Bad test
}


JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_kernel_CVE_12014_14943_checkPingPong(JNIEnv *env, jobject obj){
   return checkIsVulnerable();
}

int main(void){

  int r =  checkIsVulnerable();

  if(r == 0){
    printf("Device is vulnerable\n");
  }else if(r == 1){
    printf("Device is vulnerable\n");
  }else{
     printf("There was an error running tests\n");
  }

  return 0;
}
