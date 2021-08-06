#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

int has_run[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void runMe(int *arg)
{
    int value = (*arg);
    assert(value >= 0 && value < 5 && "Bad argument passed to 'runMe()!'");

    has_run[value] = 1;

    int *ret = (int *)malloc(sizeof(int));
    *ret = value * value;

    pthread_exit((void *)ret);
}

int run_threads(int n)
{
    pthread_t threads[n];
    int *args[n];
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        args[i] = (int *)malloc(sizeof(int));
        *args[i] = i;
        pthread_create(&threads[i], NULL, (void *)runMe, args[i]);
    }
    for (int i = 0; i < n; i++)
    {
        void *ret;
        pthread_join(threads[i], &ret);
        sum += *((int *)ret);
        free(ret);
        free(args[i]);
    }
    return sum;
}

int main(int argc, char **argv)
{

    int sum = run_threads(5);

    int correct = 0;
    for (int i = 0; i < 5; ++i)
    {
        if (has_run[i])
            correct++;
    }

    printf("%d %d", correct, sum);

    return 0;
}