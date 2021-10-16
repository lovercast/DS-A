#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <omp.h>
#include <semaphore.h>

// a program that calculates pi with pthreads.
long long n;
long long nthreads;
double sum;
int flag;
sem_t *sem;

#define error_out(msg)      \
    if (1) {                \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } else                  \
        (void) 0            


void *thread_sum(void *rank);

int main(int argc, char *argv[])
{
    // take n, nthreads as args
    assert(argc >= 3);
    n = strtoll(argv[1], NULL, 10);
    nthreads = strtoll(argv[2], NULL, 10);
    assert(n && nthreads);
    // sem_init(&sem, 0, 1); // sem_t *, , initial value
    if ((sem = sem_open("/sem", O_CREAT, 0777, 1)) == SEM_FAILED) {
        sem_close(sem);
        error_out("sem_open");
    }


    pthread_t *threads = malloc(nthreads * sizeof(pthread_t));
    if (!threads)
        perror("malloc"), exit(1);

    double tstart = omp_get_wtime();

    long ids[4] = {0};

    for (long i = 0; i < nthreads; ++i) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_sum, &ids[i]);
    }

    for (int i = 0; i < nthreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    double tend = omp_get_wtime();

    printf("%g\n", tend - tstart);
    printf("%lld\n", n);
    printf("%lld\n", nthreads);
    printf("%.20g\n", 4 * sum);


    sem_close(sem);
    free(threads);
    return 0;
}

// The stupidest code ever written!
// Same output as
// if (input % 2 == 0)
//      input = 1.;
// else
//      input = -1.
// except without jump instructions
double foo(long input)
{
    double factor = 1.;
    long *boo = (long *)(&factor);
    *boo = input % 2 << (sizeof factor * 8 - 1) | *boo;
    printf("%g", factor);
    return factor;
}

void *thread_sum(void *rank)
{
    long my_rank = *(long *)rank;
    double factor, my_sum = 0.0;
    long long i;
    long long my_n = n / nthreads;
    long long my_first_i = my_n * my_rank;
    long long my_last_i = my_first_i + my_n;
    if (my_first_i % 2 == 0)
        factor = 1.0;
    else
        factor = -1.0;
    for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
        // my_sum += factor / (2*i + 1);
        my_sum += factor / (2*i + 1);
    }

    // while (flag != my_rank) { }

    // pthread_mutex_lock(&m);
    // sem_wait(sem);
    sum += my_sum;
    // sem_post(sem);
    // pthread_mutex_unlock(&m);
    // flag = (flag + 1) % nthreads;
    return NULL;
}
/* Thread sum */