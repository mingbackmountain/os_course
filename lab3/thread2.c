#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *hello(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Hello world from %ld \n", tid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t thread[NUM_THREADS];
    int status;
    long t;

    for (t = 0; t < NUM_THREADS; t++)
    {
        printf("Main program creates thread %ld \n", t);
        status = pthread_create(&thread[t], NULL, hello, (void *)t);
        if (status)
        {
            printf("ERROR - return code from thread cread is %d\n", status);
            exit(-1);
        }
    }
    pthread_exit(NULL);
}