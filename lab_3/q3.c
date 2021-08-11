#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    int fd;
    if ((fd = open("my.file", O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
    {
        perror("Couldn't open my.file");
        exit(1);
    }

    /* Redirect stdin to my.file */

    dup2(fd, STDIN_FILENO);
    // close(fd);

    execl("/usr/bin/sort", "sort", "-k+7", NULL);
}