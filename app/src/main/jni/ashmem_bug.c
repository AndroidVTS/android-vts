#include <stdio.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <linux/ashmem.h>
#include <sys/mman.h>
#include <jni.h>

#define ASHMEM_DEVICE   "/dev/ashmem"

/*
 * ashmem_create_region - creates a new ashmem region and returns the file
 * descriptor, or <0 on error
 *
 * `name' is an optional label to give the region (visible in /proc/pid/maps)
 * `size' is the size of the region, in page-aligned bytes
 */
int ashmem_create_region(const char *name, size_t size)
{
        int fd, ret;

        fd = open(ASHMEM_DEVICE, O_RDWR);
        if (fd < 0)
                return fd;

        if (name) {
                char buf[ASHMEM_NAME_LEN];

                strlcpy(buf, name, sizeof(buf));
                ret = ioctl(fd, ASHMEM_SET_NAME, buf);
                if (ret < 0)
                        goto error;
        }

        ret = ioctl(fd, ASHMEM_SET_SIZE, size);
        if (ret < 0)
                goto error;

        return fd;

error:
        close(fd);
        return ret;
}



int ashmem_set_prot_region(int fd, int prot)
{
        return ioctl(fd, ASHMEM_SET_PROT_MASK, prot);
}

int checkIsVulnerable(){
  int fd;
  int size = 0x100;
   char * data;

   fd = ashmem_create_region("test_region", size);
   if(fd < 0)
      goto error;

  data = (char *)  mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(data == MAP_FAILED)
    goto error;

  memset(data, 'B', size);

  /* allow the wolves we share with to do nothing but read */
  ashmem_set_prot_region(fd, PROT_READ);

  void * map2 = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);

  if(map2 == MAP_FAILED){
    printf("Child map failed\n");
    goto error;
  }

  if (mprotect(map2, size, PROT_READ|PROT_WRITE) < 0) {
        printf("[-] mprotect errno:  %d\n", errno);
        perror("mprotect");

        if(errno == 13) { //Permission denied - patched
          goto not_vulnerable;
        }
        //Unknown error
        goto error;
  }

  memset(map2, 'A', size);

  if(strncmp(data, "AAAAAAAAAAAAAAAA", 10) == 0){
    //We were able to successfully write to our original ashmem mapping after protecting it
    goto vulnerable;
  }

  goto error;

vulnerable:
  printf("Vulnerable\n");
  close(fd);
  return 1;

not_vulnerable:
  printf("Not Vulnerable\n");
  close(fd);
  return 0;

error:
  printf("Error\n");
  close(fd);
  return -1;
}

JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_kernel_CVE_12011_11149_checkASHMemMap(JNIEnv *env, jobject obj){
   return checkIsVulnerable();
}

int main(void){
  return checkIsVulnerable();
}
