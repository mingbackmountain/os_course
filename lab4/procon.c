#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>

typedef int buffer_item;
#define BUFFER_SIZE 10

#define RAND_DIVISOR 1000000
#define TRUE 1

pthread_mutex_t mutex;

sem_t full, empty;

buffer_item buffer[BUFFER_SIZE];

int counter;

pthread_t tid;
pthread_attr_t attr;

void *producer(void *pno);
void *consumer(void *cno);

void initializeData()
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    pthread_attr_init(&attr);

    counter = 0;
}

int insert_item(buffer_item item)
{
    if (counter < BUFFER_SIZE)
    {
        buffer[counter] = item;
        counter++;
        return 0;
    }
    else
    {
        return -1;
    }
}

int remove_item(buffer_item *item)
{
    if (counter > 0)
    {
        *item = buffer[counter - 1];
        counter--;
        return 0;
    }
    else
    {
        return -1;
    }
}

void *producer(void *pno)
{
    buffer_item item;
    long tid;
    tid = (long)pno;

    while (TRUE)
    {
        item = (buffer_item)rand() % 100;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        if (insert_item(item))
        {
            fprintf(stderr, " Producer report error condition\n");
        }
        else
        {
            printf("producer no %ld produced %d\n", tid, item);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *cno)
{

    buffer_item item;
    long tid;

    tid = (long)cno;

    while (TRUE)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        if (remove_item(&item))
        {
            fprintf(stderr, "Consumer report error condition\n");
        }
        else
        {
            printf("consumer no %ld consumed %d\n", tid, item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main(int argc, char *argv[])
{
    long i;
    if (argc != 4)
    {
        fprintf(stderr, "USAGE:./main.out <INT> <INT> <INT>\n");
        exit(-1);
    }
    int mainSleepTime = atoi(argv[1]);
    int numProd = atoi(argv[2]);
    int numCons = atoi(argv[3]);

    initializeData();

    for (i = 0; i < numProd; i++)
    {
        pthread_create(&tid, &attr, producer, (void *)i);
    }

    for (i = 0; i < numCons; i++)
    {
        pthread_create(&tid, &attr, consumer, (void *)i);
    }

    sleep(mainSleepTime);

    printf("Exit the program\n");
    exit(0);
}
