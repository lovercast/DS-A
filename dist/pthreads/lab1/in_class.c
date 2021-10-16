#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LIMIT 10

void *hello_world(void *);

int main(int argc, char *argv[])
{
    int i;
    long num_threads = 0;
    pthread_t *thread_array;
    long *thread_ids;

    if (argc > 2)
        num_threads = strtol(argv[1], NULL, LIMIT);

    printf("number %ld\n", num_threads);

    return 0;

    thread_array = malloc(num_threads * sizeof(pthread_t));
    thread_ids = malloc(num_threads * sizeof(long));

    for (i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&thread_array[i], NULL, hello_world, &thread_ids[i]);
    }


    free(thread_array);
    free(thread_ids);


}

void *hello_world(void *id)
{
    long *myid = (long *)id;
    printf("hello, I am thread %ld\n", *myid);
    return NULL;
}
