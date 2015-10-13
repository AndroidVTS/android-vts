#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

static int BAD_COMMAND_STATUS = 42;
static pid_t childPid = 0;

void quit(int code) {
  if (childPid > 0) {
    kill(childPid, SIGKILL);
    exit(code);
  }
}

void child_handler(int sig) {
  int   status;
  pid_t pid = waitpid(childPid, &status, WNOHANG);
  if (pid == -1) {
    printf("Child didn't exit wat\n");
    quit(-1);
  }

  if (WIFEXITED(status)) {
    if (WEXITSTATUS(status) == BAD_COMMAND_STATUS) {
      printf("Error executing child process\n");
      quit(-1);
    } else {
      printf("Child exited normally\n");
      quit(WEXITSTATUS(status));
    }
  } else {
    printf("Boom goes the dynamite\n");
    quit(-2);
  }
}

int main(int argc, char** argv, char** envp) {
  if (argc < 3) {
    printf("Usage: crashCheck wait_seconds program [args]\n");
    exit(-1);
  }

  int numSeconds = atoi(argv[1]);
  if (numSeconds == 0) {
    printf("Bad timeout value\n");
    exit(-1);
  }

  pid_t pid = fork();
  if (pid == 0) {
    execve(argv[2], &argv[2], envp);
    return BAD_COMMAND_STATUS;
  } else {
    childPid = pid;

    struct sigaction sa;
    bzero(&sa, sizeof(sa));
    sa.sa_handler = child_handler;
    sigaction(SIGCHLD, &sa, NULL);

    sleep(numSeconds);

    printf("Child still running\n");
    quit(-3);
    return 0;
  }
}
