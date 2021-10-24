/* Generalize the previous problem so that
 * each thread is both a producer and a consumer.
 * Author: Levi Overcast
 */

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

long nthreads;
char **messages;
int *availability;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// don't need this! enum { PRODUCER = 0, CONSUMER = 1 };

void *thread_fun(void *);
void create_message(long, long);
void print_message(long);
void clean_up(void);

int main(int argc, char *argv[])
{
    long i, j, err;
    assert(argc >= 2);
    nthreads     = strtol(argv[1], NULL, 10);
    messages     = malloc(nthreads * sizeof(char *));
    availability = calloc(nthreads, sizeof(int));
    pthread_t tid[nthreads];

    for (i = 0; i < nthreads; ++i) {
        err = pthread_create(&tid[i], NULL, thread_fun, (void *)i);
        assert(err == 0);
    }

    for (i = 0; i < nthreads; ++i) {
        err = pthread_join(tid[i], NULL);
        assert(err == 0);
    }

    clean_up();
}

void clean_up()
{
    for (int i = 0; i < nthreads / 2; ++i) {
        free(messages[i]);
    }
    free(messages);
    free(availability);
}

void *thread_fun(void *v)
{
    long my_rank = *(long *)v;
    long to_rank = (my_rank + 1) % nthreads;

    while (1) {
        pthread_mutex_lock(&mutex);
        if (availability[my_rank]) {
            print_message(my_rank);
            pthread_mutex_unlock(&mutex);
            break;
        }

        create_message(my_rank, to_rank);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

/* produce */
void create_message(long from, long to)
{
    if (availability[to] == 0) {
        char *msg = malloc(sizeof(char) * 128);
        assert(msg);
        int ret = sprintf(msg, "hello from thread %ld!", from);
        assert(ret);
    }
    availability[to] = 1;
}

/* consume */
void print_message(long idx)
{
    printf("%s\n", messages[idx]);
    //  free(messages[idx]);
}
