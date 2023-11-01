#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#define PROCESS_FILE_NAME "/var/run/laba4daemon.pid"

int createFile(char *, int);
void closeFile(int);
void printErrno();

int main(int cargs, char ** vargs) {
    int childPID;
    if ((childPID = fork()) == -1 ) {
        perror ("Can 't fork"); exit(1); }
    else if (childPID == 0 ) {
        /* child */
        int pid = getpid();
        printf ("Сhild:PID=%d,PPID=%d\n", pid ,getppid());

        int fd;
        if((fd = createFile(PROCESS_FILE_NAME, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH)) < 0) {
            printErrno();
            exit(0);
        }

        char buff[15];
        sprintf(buff, "%d", pid);

        if (write(fd,buff,strlen(buff)) < 0) {
            printErrno();
            exit(0);
        }

        closeFile(fd);

        setsid();

        while (1) {
            pause();
        }
    } else {
        /* parent */
        printf ("Parent:childPID=%d, PID=%d, PPID=%d\n",
                childPID,getpid(),getppid());
        exit(0);
    }
}

int createFile(char * fileName, int mode)
{
    int fileDescriptor = creat(fileName, mode);
    if(fileDescriptor == -1) {
        printErrno();
        exit(1);
    }

    return fileDescriptor;
}

void closeFile(int fileDescriptor)
{
    if(-1 == close(fileDescriptor)) {
        printErrno();
        exit(0);
    }
}

void printErrno()
{
    printf("errno_№: %d, error_message: %s\n", errno, strerror(errno));
}