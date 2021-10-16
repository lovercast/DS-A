// matrix vector multiplication
// divide the matrix between threads and assign each thread a portion of the product vector for which it is responsible.
// How to integrate the data that they all write?
// Store each piece in a common place. Need to know how long each portion is.
// Write a method to copy the data into the final product vector.
// Schedule input and output? What does that mean?


#include "matvect.h"

// #ifndef SHARED
// #define SHARED
// #endif

#ifdef SHARED
#undef SHARED
#endif

#ifdef SHARED
double *A, *x, *y;
#endif

int main(int argc, char *argv[])
{
    int i, j;

    assert(argc >= 4);
    long M = strtol(argv[1], NULL, 10);
    long N = strtol(argv[2], NULL, 10);
    long nthreads = strtol(argv[3], NULL, 10);

    pthread_t *thread_arr;  // cannot assign to non-static member [member] with const qualified type
    struct mat_arg *arg_arr;

    thread_arr = xmalloc(nthreads * sizeof(pthread_t));
    arg_arr = xmalloc(nthreads * sizeof(struct mat_arg)); // TODO fix the logic here -- should be tied to the number of threads, not the number of dims.

#ifndef SHARED
    double *A, *x, *y;
#endif

    A = xmalloc(M*N*sizeof(double)); // matrix
    x = xmalloc(N * sizeof(double)); // vector
    y = xmalloc(N * sizeof(double));


    // allocate individual pieces of the input matrix 
    // array of tiny arrays, each with num columns the same, 
    // copy the data over to the copies
#ifndef SHARED
    double **A_chunks = xmalloc(nthreads * sizeof(double *));
    double **x_chunks = xmalloc(nthreads * sizeof(double *));
#endif
    // need to divide the matrix here! might as well pass in the dimensions with the struct
    int stride = M / nthreads;
    // for each thread, calculate how many rows it gets and create copies of the rows that they will alter
    int row0, row1, nrows;
    for (i = 0; i < nthreads; ++i) {
        row0 = i * stride;
        row1 = (i < nthreads - 1) ? row0 + stride : M; // exclusive upper bound
        nrows = row1-row0;

#ifndef SHARED
        A_chunks[i] = xmalloc(nrows * N * sizeof(double));
        // copy the data from our input matrix to our chunk
        memmove(A_chunks[i], &A[row0], nrows * N * sizeof(double));
        x_chunks[i] = xmalloc(nrows * sizeof(double));
#endif

        // initialize corresponding thread arg
        arg_arr[i].N = N;
        arg_arr[i].id = i;
        arg_arr[i].row0 = row0;
        arg_arr[i].row1 = row1;
        arg_arr[i].thread_time = 0;
        arg_arr[i].rows = nrows;   
#ifndef SHARED
        arg_arr[i].A = A_chunks[i]; // with distribution
        arg_arr[i].y = x_chunks[i];
#else
        arg_arr[i].A = A; // without distribution
        arg_arr[i].y = y;
#endif
        arg_arr[i].x = x; 
    }


    // time to create the dang threads!
    double tstart = omp_get_wtime();
    for (i = 0; i < nthreads; ++i) 
        pthread_create(&thread_arr[i], NULL, thread_fun, &arg_arr[i]);
    for (i = 0; i < nthreads; ++i) 
        pthread_join(thread_arr[i], NULL);
    
    // copy the result to the output vector
    // for each thread, go to its argument struct, 
    // copy arg->rows rows of data into y
#ifndef SHARED
    for (i = 0; i < nthreads; ++i) 
        memmove(&y[arg_arr[i].row0], arg_arr[i].y, arg_arr[i].rows * sizeof(double));
#endif
    double tend = omp_get_wtime();

    double total_time = tend - tstart;

    // print_vect(N, y);
    printf("Num threads: %ld\n", nthreads);
    printf("Num rows: %ld\n", M);
    printf("Num cols: %ld\n", N);
    printf("Total time: %g\n", total_time);
    traverse_free(head);        
    return 0;
}


/* clean up */
void traverse_free(struct llist *self)
{
    if (self == NULL) 
        return;
    free(self->ptr);
    traverse_free(self->next);
    free(self);
}

 
// malloc wrapper
// check for ENOMEM
// add allocated pointer to list of allocated and increment number of allocated.
void *xmalloc(size_t size)
{
    void *ptr = malloc(size);
    struct llist *new_list = malloc(sizeof(struct llist));
    if (!ptr || !new_list)
        perror("malloc"), exit(1);
    if (head == NULL) {
        head = new_list;
        current = new_list;
    } else {
        current->next = new_list;
        current = new_list;
    }
    new_list->ptr = ptr;
    new_list->next = NULL;
    return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    if (!ptr)
        eprintf("Realloc failed!");
    return ptr;
}

void *thread_fun(void *v)
{
    int i, j;
    int N, id, row0, row1;
    double thread_time, rows, *A, *x;
    struct mat_arg *arg = (struct mat_arg *)v;

    // write a script to line up the equals signs in a block of assignment code
    row0        = arg->row0;
    row1        = arg->row1;


    double tstart = omp_get_wtime();

    // for each row, calculate dot product, write it to the corresponding row in the output vector
    for (i = row0; i < row1; ++i) {
        for (j = 0; j < N; ++j) {
#ifndef SHARED
            arg->y[i-row0] += *(arg->A + (i - row0) * N + j) * arg->x[j]; 
#else
            y[i] += *(A + i * N + j) * x[j]; 
#endif
        }
    }

    double tend = omp_get_wtime();
    thread_time = tend - tstart;

    return &arg->rows;
}

void print_vect(int N, double *a)
{
    printf("[ ");
    for (int i = 0; i < N; ++i) {
        printf("%g ", a[i]);
    }
    printf("]\n");
}

inline void print_mat(const int M, const int N, const double (*restrict X)[M][N])
{
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (j == 0)
                printf("[");
            printf("%3g ",(*X)[i][j]);
            if (j == N-1)
                printf("]\n");
        }
    }
}

#ifdef SHARED
#undef SHARED
#endif