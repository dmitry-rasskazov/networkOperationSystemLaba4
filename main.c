#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define PROCESS_FILE_NAME "/var/run/laba4daemon.pid"

void printErrno();

int main(int cargs, char ** vargs) {
    int childPID;
    if ((childPID = fork()) == -1 ) {
        perror ("Can 't fork"); exit(1); }
    else if (childPID == 0 ) {
        /* child */
        int pid = getpid();
        printf ("Сhild:PID=%d,PPID=%d\n", pid ,getppid());

        FILE *file;
        if((file = fopen(PROCESS_FILE_NAME, "w")) == NULL) {
            printErrno();
            exit(0);
        }

        if (fprintf(file,"%d", pid) < 0) {
            printErrno();
            exit(0);
        }

        fclose(file);

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

void printErrno()
{
    printf("errno_№: %d, error_message: %s\n", errno, strerror(errno));
}