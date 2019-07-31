#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "asroot.h"

#define BIN "wunderbar"
#define BIN_PATH "/data/data/com.duosecurity.xray/files/" BIN

int checkIsVulnerable()
{
	FILE *fp;
	int result;
	int i, pid, ret, status = -1;

	umask(007);

	fp = fopen(BIN_PATH, "wb");
	if (!fp) {
		// patched|failed to open file
		result = 0;
		goto done;
	}
	for (i = 0; i < sizeof(asroot) / sizeof(uint32_t); ++i) {
		fwrite(&asroot[i], sizeof(uint32_t), 1, fp);
	}
	fclose(fp);

	ret = chmod(BIN_PATH, 0750);
	if (ret == -1) {
		// patched|failed to chmod file
		result = 0;
		goto unlink;
	}

	pid = fork();
	if (pid == -1) {
		// patched|failed to fork
		result = 0;
		goto unlink;
	}

	if (pid == 0) {
		ret = execl(BIN_PATH, BIN, NULL);
		if (ret == -1) {
			// patched|failed to exec
			result = 0;
			goto kill;
		}
		exit(1);
	}

	sleep(1);

	ret = waitpid(pid, &status, WNOHANG);
	if (ret == -1) {
		// patched|failed to waitpid
		result = 0;
		goto kill;
	}

	if (WIFEXITED(status)) {
		// patched|bin exited normally
		result = 0;
		goto kill;
	}

	if (WIFSIGNALED(status)) {
		// patched|bin was killed
		result = 0;
		goto kill;
	}

	// vulnerable|bin wasn't terminated
	result = 1;

kill:
	kill(pid, SIGKILL);

unlink:
	unlink(BIN_PATH);

done:
	return result;
}

JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_kernel_CVE_12009_12692_checkWunderbar(JNIEnv *env, jobject obj){
   return checkIsVulnerable();
}

int main(void){
  return checkIsVulnerable();
}
