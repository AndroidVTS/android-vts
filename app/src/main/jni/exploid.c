#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#define DEV_NODE "exploid"
#define DEV_PATH "/dev/" DEV_NODE

int checkIsVulnerable()
{
	int ret, sock, result;
	char buf[64];
	struct sockaddr_nl snl;
	struct iovec iov = { buf, sizeof(buf) };
	struct msghdr msg = { &snl, sizeof(snl), &iov, 1, NULL, 0, 0 };
	struct stat sbuf;

	memset(&snl, 0, sizeof(snl));
	snl.nl_pid = 1;
	snl.nl_family = AF_NETLINK;

	sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (sock == -1) {
		// patched | can't create socket
		result = 0;
		goto done;
	}

	snprintf(buf, sizeof(buf), "ACTION=add%cDEVPATH=/" DEV_NODE "%cSUBSYSTEM=exploid%cMAJOR=1%cMINOR=1%c", 0, 0, 0, 0, 0);

	ret = sendmsg(sock, &msg, 0);
	if (ret == -1) {
		// patched | can't send payload
		result = 0;
		goto close;
	}

	sleep(1);

	ret = stat(DEV_PATH, &sbuf);
	if (ret == -1) {
		// patched | can't find exploid device
		result = 0;
	} else {
		// vulnerable | found exploid device
		result = 1;
	}

	snprintf(buf, sizeof(buf), "ACTION=remove%cDEVPATH=/" DEV_NODE "%cSUBSYSTEM=exploid%cMAJOR=1%cMINOR=1%c", 0, 0, 0, 0, 0);

	ret = sendmsg(sock, &msg, 0);
	if (ret == -1) {
		// patched | can't send payload
		result = 0;
		goto close;
	}

close:
	close(sock);

done:
	return result;
}

JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_kernel_CVE_12009_11185_checkExploid(JNIEnv *env, jobject obj){
   return checkIsVulnerable();
}

int main(void){
  return checkIsVulnerable();
}
