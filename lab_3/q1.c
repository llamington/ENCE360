#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#define INP 1
#define OUTP 0

int main()
{
    int fd[2];
    pipe(fd);
    if (fork() == 0)
    {
        dup2(fd[INP], STDOUT_FILENO);
        close(fd[OUTP]);

        execl("/usr/bin/sort", "sort", NULL);
    }
    else
    {
        dup2(fd[OUTP], STDIN_FILENO);
        close(fd[INP]);
        execl("/usr/bin/head", "head", "-n", "2", NULL);
        int stat;
        wait(&stat);
    }
}