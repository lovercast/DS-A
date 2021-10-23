#include "pacheco_matvect.h"

// test time with array as a two dimensional array and as a one dimensional
// array

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nthreads>\n", argv[0]);
        exit(1);
    }

    int i, j, k;
    pthread_t tid[nthreads];
    pthread_attr_t attr;

    nthreads = atoi(argv[1]);

    identify(A);
    identify_1d(B);
    init_x(x);

    // start wall time
    double start = omp_get_wtime();
    // Initialize and set thread joinable
    for (i = 0; i < nthreads; i++) {
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&tid[i], &attr, Pth_mat_vect_A, (void *)i);
    }

    for (i = 0; i < nthreads; i++)
        pthread_join(tid[i], NULL);

    // end wall time
    double end = omp_get_wtime();

    double w1 = end - start;

    pthread_t tid2[nthreads];

    // start wall time
    start = omp_get_wtime();

    // Initialize and set thread joinable
    for (i = 0; i < nthreads; i++) {
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&tid2[i], &attr, Pth_mat_vect_B, (void *)i);
    }

    // join threads
    for (i = 0; i < nthreads; i++)
        pthread_join(tid2[i], NULL);
    end = omp_get_wtime();

    double w2 = end - start;

    // assert the result vector is all ones
    assert_ones(y1);
    printf("nthreads: %d\n", nthreads);
    printf("%g\n", w2); // 1d array
    printf("%g\n", w1); // 2d array
    // assert the result vector is all ones
    assert_ones(y2);
    printf("\n");
}

// assert the input vector is all ones
void assert_ones(int v[n])
{
    for (int i = 0; i < n; i++)
        assert(y1[i] == 1);
}

// initialize A to be the identity matrix
void identify(int A[m][n])
{
    int i, j;
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            A[i][j] = (i == j);
}

// initialize one dimensional array to be the identity matrix
void identify_1d(int A[m * n])
{
    int i;
    for (i = 0; i < m * n; i++)
        A[i] = (i == i / n + i % n * m);
}

// initialize x to be a vector of 1s
void init_x(int x[n])
{
    int i;
    for (i = 0; i < n; i++)
        x[i] = 1;
}

// with array as a two dimensional array
void *Pth_mat_vect_A(void *rank)
{
    int my_rank = (int)rank;
    int i, j;
    int local_m = m / nthreads;
    int i_start = my_rank * local_m;
    int i_stop  = (my_rank < nthreads - 1) ? i_start + local_m : m;
    int local_y[i_stop - i_start];
    for (i = i_start; i < i_stop; i++) {
        local_y[i - i_start] = 0;
        for (j = 0; j < n; j++)
            local_y[i - i_start] += A[i][j] * x[j];
    }

    for (i = i_start; i < i_stop; ++i)
        y1[i] = local_y[i - i_start];
    return NULL;
}
/* Pth mat vect */

// with array as a one dimensional array
void *Pth_mat_vect_B(void *rank)
{
    long my_rank = (long)rank;
    int i, j;
    int local_m = m / nthreads;
    int i_start = my_rank * local_m;
    int i_stop  = (my_rank < nthreads - 1) ? i_start + local_m : m;
    int local_y[i_stop - i_start];
    for (i = i_start; i < i_stop; i++) {
        local_y[i - i_start] = 0;
        for (j = 0; j < n; j++)
            local_y[i - i_start] += B[i * n + j] * x[j];
    }

    // memcpy(&y2[i_start], &local_y[0], (i_stop - i_start) * sizeof(int));
    for (i = i_start; i < i_stop; ++i)
        y2[i] = local_y[i - i_start];
    return NULL;
}
/* Pth mat vect */

// print vector
void print_x(int x[n])
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", x[i]);
    printf("\n");
}