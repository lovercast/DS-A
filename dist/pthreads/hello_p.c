#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *HelloWorld(void *);

int main(int argc, char *argv[])
{
    // if (argc < 2)
    //     return 0;
    // int num_threads = atoi(argv[1]);
    int num_threads = 4;
    pthread_t *thread_array;
    char thread_args[num_threads][64];

    thread_array = malloc(num_threads * sizeof(pthread_t));

    int i;

    for (i = 0; i < num_threads; i++) {
        // thread_ids[i] = i;
        if (argc >= i+1)
            strncpy(&thread_args[i][0], argv[i+2], 10);
        else
            thread_args[i][0] = i - '0';


        pthread_create(&thread_array[i], NULL, HelloWorld, &thread_args[i]);
    }

    long receiver[num_threads]; // variable sized object may not be initialized (?)

    for (i = 0; i < num_threads; i++) {
        pthread_join(thread_array[i], &receiver[i]);
        printf("Hi there: %ld\n", receiver[i]);
    }

}

void *HelloWorld(void *arg)
{
    printf("%s\n", (char *)arg);
    printf("hello world\n");
    long l = 100;
    return &l;
}
