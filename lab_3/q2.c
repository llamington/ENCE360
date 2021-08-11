#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int count = 0;

void handle_sigquit()
{
    signal(SIGQUIT, handle_sigquit);
    write(1, "SIGQUIT\n", 8);
    if (++count >= 2)
    {
        exit(0);
    }
}

void install_handler()
{
    signal(SIGQUIT, handle_sigquit);
}

void main()
{
    int pid;
    if ((pid = fork()) == 0)
    {
        install_handler();
        while (1)
            ;
    }
    else
    {
        sleep(1);
        kill(pid, SIGQUIT);
        sleep(1);
        kill(pid, SIGQUIT);
    }
}