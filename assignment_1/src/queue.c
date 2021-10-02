
#include "queue.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#define handle_error_en(en, msg) \
    do                           \
    {                            \
        errno = en;              \
        perror(msg);             \
        exit(EXIT_FAILURE);      \
    } while (0)

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

typedef struct QueueStruct
{
    void **array;
    int length; // current length of queue
    sem_t items;
    sem_t spaces;
    pthread_mutex_t lock;
} Queue;

Queue *queue_alloc(int size)
{
    // allocate memory for the queue
    Queue *queue = malloc(sizeof(Queue));
    queue->array = malloc(size * sizeof(void *));
    queue->length = 0;

    pthread_mutex_init(&queue->lock, NULL);

    // initiliase items semaphore with 0
    if (sem_init(&queue->items, 0, 0) == -1)
        handle_error("sem_init items");

    // initialise spaces semaphore with the number of free spaces
    if (sem_init(&queue->spaces, 0, size) == -1)
        handle_error("sem_init spaces");

    return queue;
}

void queue_free(Queue *queue)
{
    free(queue->array);
    free(queue);
}

void queue_put(Queue *queue, void *item)
{
    // TODO: mutex?

    if (sem_wait(&queue->spaces) == -1)
        handle_error("sem_wait spaces");

    pthread_mutex_lock(&queue->lock);

    // add the item to the array, incrementing the length
    queue->array[queue->length++] = item;

    pthread_mutex_unlock(&queue->lock);

    if (sem_post(&queue->items) == -1)
        handle_error("sem_post items");
}

void *queue_get(Queue *queue)
{
    // TODO: mutex?

    if (sem_wait(&queue->items) == -1)
        handle_error("sem_wait spaces");

    pthread_mutex_lock(&queue->lock);

    void *res = queue->array[0];

    // shift the queue positions towards the front by 1
    for (int i = 1; i < queue->length; i++)
        queue->array[i - 1] = queue->array[i];
    queue->length--;

    pthread_mutex_unlock(&queue->lock);

    // increase the number of free spaces in the queue
    if (sem_post(&queue->spaces) == -1)
        handle_error("sem_post spaces");

    return res;
}
