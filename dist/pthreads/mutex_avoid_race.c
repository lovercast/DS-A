#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
// use timer to time the functions

// declare global mutex

// create a thread function with pthread_mutex_lock and pthread_mutex_unlock

// in main, initialize the mutex before running pthread_create, 
// after running pthread_join, call pthread_mutex_destroy(&mutex)

// what is initialize doing? How do the mutex locks work?

struct timeval timer;
pthread_mutex_t mutex;

void *hello(void *);

int main()
{
    long a = 4;
    long b = 10;
    long result = a / b + (a % b != 0);
    printf("%ld\n", result);

    return 0;

    int i;
    pthread_mutex_init(&mutex, NULL);
    long nthreads = 4;
    pthread_t *p = malloc(nthreads*sizeof(pthread_t));

    /* pthread_create */
    for (i = 0; i < nthreads; i++) {
        pthread_create(&p[i], NULL, hello, NULL);
    }

    /* pthread_join */
    for (i = 0; i < nthreads; i++) {
        pthread_join(p[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}

void *hello(void *arg)
{
    // timer start
    gettimeofday(&timer, NULL);
    suseconds_t tstart = timer.tv_usec;

    // body of code
    for (long i = 0; i < 100000000; i++);

    // timer stop
    gettimeofday(&timer, NULL);
    suseconds_t tend = timer.tv_usec;

    printf("total time in microseconds: %d\n", tend-tstart);
    return NULL;
}