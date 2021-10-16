#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

pthread_mutex_t mutex; // global declaration of mutex -- initialized in main

static long important_thing;

sem_t *sem;

void *critical_code(void *arg)
{

    sem = sem_open("hello", 1, 0);

    sem_close(sem);

    // get id
    pthread_mutex_lock(&mutex);

    long *myid = (long *) arg;
    important_thing += *myid;
    printf("The important thing is %ld\n", important_thing);
    printf("Thread id %ld very sleepy.............", *myid);
    sleep(1);
    printf(" Wakey wakey\n");

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char *argv[])
{
    // note to self -- write a script to shart out boiler plate so I don't have to do it every time.
    pthread_mutex_init(&mutex, NULL); // initialize the mutex -- does this mean it will prevent other threads from accessing what's below this?

    long nthreads = 4;
    pthread_t *thread_arr = malloc(nthreads * sizeof(pthread_t));
    long * thread_ids = malloc(nthreads * sizeof(long));

    // assign ids and create threads
    int i;

    for (i = 0; i < nthreads; i++) {
        thread_ids[i]  = i;
        pthread_create(&thread_arr[i], NULL, critical_code, &thread_ids[i]);
    }

    for (i = 0; i < nthreads; i++){ 
        pthread_join(thread_arr[i], NULL);
    }

    pthread_mutex_destroy(&mutex); // after calling pthread_join -- destroy the mutex lock

    free(thread_arr);
    free(thread_ids);

}