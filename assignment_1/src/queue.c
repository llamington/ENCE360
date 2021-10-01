
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
    sem_t items;
    sem_t spaces;

} Queue;

Queue *queue_alloc(int size)
{
    // allocate memory for the queue
    Queue *queue = malloc(sizeof(Queue));
    queue->array = malloc(size * sizeof(void *));

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

    // Get the number of items in queue
    int num_items;
    if (sem_getvalue(&queue->items, &num_items) == -1)
        handle_error("sem_getvalue items");

    // add the item to the array
    queue->array[num_items] = item;
    if (sem_post(&queue->items) == -1)
        handle_error("sem_post items");
}

void *queue_get(Queue *queue)
{

    assert(0 && "not implemented yet!");
}
