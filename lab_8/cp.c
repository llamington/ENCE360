#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];


int main(int argc, char *argv[])
{
    int src_fd;
    int dest_fd;

    if (argc != 3){
        printf(1, "usage: cp src_file dest_file\n");
        exit();
    }

    src_fd = open(argv[1], O_RDONLY);
    dest_fd = open(argv[2], O_WRONLY | O_CREATE);

    int s;
    do {
        s = read(src_fd, buf, 512);
        if(s == -1) {
            printf(1, "cp: read error\n");
            exit();
        }
        
        s = write(dest_fd, buf, s);
        if(s == -1) {
            printf(1, "cp: write error\n");
            exit();
        }


    } while(s > 0);

    exit();
}
