#include "types.h"
#include "stat.h"
#include "user.h"

#define NULL 0


int main(void)
{
    int p[2];
    int p2[2];

    pipe(p2);
    pipe(p);
    int pid = fork();

    if (pid == 0) {

        // Redirect stdin
        close(0);
        dup(p[0]);

        // redirect stdout
        close(1);
        dup(p2[1]);

        close(p2[0]);
        close(p2[1]);
        close(p[0]);
        close(p[1]);

        char *argv[] = {"wc", NULL};
        exec("wc", argv);

    } else {

        write(p[1], "hello world\n", 12);

        close(p[0]);
        close(p[1]);

        close(p2[1]);

        char buf[20];
        int n;
        int i = 0;
        
        do {
            n=read(p2[0], buf+i++, 1);
        } while(n>0);


        printf(1, "%s", buf);
        close(p2[0]);

        wait();
    }

    exit();
}
