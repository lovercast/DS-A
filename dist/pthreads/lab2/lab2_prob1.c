#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LIM 100

/*
 * problem 1 is matrix multiplication
 */

struct t_arg {
    int arrlen;
    long scalar;
    long thread_id;
    long nthreads;
    long *arr;
};

#define errorf(...) (fprintf(stderr, __VA_ARGS__), exit(1))

void *vec_mul(void *);
void gen_random(long arr[], int arrlen, int abclen);
void print_arr(long arr[], int arrlen, int width);


int main(int argc, char *argv[])
{
    int i;
    long *arr;
    pthread_t *thread_arr;
    struct t_arg *thread_args;
    long scalar;


    if (argc < 3)
        errorf("you made a mistake buddy");
    long arrlen = strtol(argv[1], NULL, LIM);
    long nthreads = strtol(argv[2], NULL, LIM);

    arr = malloc(arrlen * sizeof(long));
    thread_arr = malloc(nthreads * sizeof *thread_arr);
    thread_args = malloc(nthreads * sizeof(struct t_arg));

    srand(42);
    scalar = rand() % 100;
    printf("%ld\n", scalar);

    gen_random(arr, arrlen, LIM);

    /* print before */
    print_arr(arr, arrlen, 20);

    for (i = 0; i < nthreads; i++) {
        thread_args[i].arrlen = arrlen;
        thread_args[i].scalar  = scalar;
        thread_args[i].thread_id = i;
        thread_args[i].nthreads = nthreads;
        thread_args[i].arr = arr;
        pthread_create(&thread_arr[i], NULL, vec_mul, &thread_args[i]);
    }


    for (i = 0; i < nthreads; i++) {
        pthread_join(thread_arr[i], NULL);
    }

    /* print after */
    print_arr(arr, arrlen, 20);

    free(arr);
    free(thread_arr);
    free(thread_args);

    return EXIT_SUCCESS;
}

void gen_random(long arr[], int arrlen, int abclen)
{
    int i;

    for (i = 0; i < arrlen; i++)
        arr[i] = rand() % abclen;
}


// returns a * X for a in R1, X in R^arrlen
void *vec_mul(void *v)
{
    struct t_arg *arg = (struct t_arg *)v;
    int arrlen = arg->arrlen;
    int nthreads = arg->nthreads;
    int start_i, end_i;
    long chunk_size; // total size of leap from one starting idx to the next
    chunk_size = arrlen/nthreads;
    start_i = arg->thread_id * chunk_size;
    end_i = (arg->thread_id + 1) * chunk_size;
    if (arg->thread_id == nthreads - 1)
        end_i = arrlen;

    int i;
    for (i = start_i; i < end_i; i++) {
        arg->arr[i] = arg->arr[i] * arg->scalar;
    }
    return NULL;
}

void print_arr(long arr[], int arrlen, int width)
{
    printf("[");
    for (int i = 0; i < arrlen; i++) {
        if (i % width == 0 && i != 0)
            printf("[");
        printf("%3ld ", arr[i]);
        if (i % width == width - 1 && i != arrlen - 1)
            printf("]\n");
    }
    printf("]\n");
}