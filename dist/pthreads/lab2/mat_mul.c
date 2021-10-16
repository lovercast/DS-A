/* Efficient matrix multiplication */
/* Based on an algorithm in the Drepper book */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <sys/sysctl.h>
#include <omp.h>

#define eprintf(...) (fprintf(stderr, __VA_ARGS__), exit(1))

/* Function prototypes */
static inline void mat_mul(const int M, const int N,
            const double (*restrict X)[M][N],
            const double (*restrict Y)[N][M],
                  double (*restrict Z)[M][M]);
static inline void matrix_mul_ordinary(const int M, const int N, const int i_start, const int i_stop,  // alternative algorithm
            const double (*restrict X)[M][N],
            const double (*restrict Y)[N][M],
                  double (*restrict Z)[M][M]);
static inline double *get_random_matrix(const int row, const int col);
void print_mat(const int m, const int n, const double (*X)[m][n]);
static inline void *xmalloc(size_t size);
void *thread_fun(void *v);
static inline void clean_up();
static inline void *xrealloc(void *ptr, size_t size);
void identify(double *X, int len);

int64_t CLS;
size_t len = sizeof CLS;

struct mat_arg {
    int M;
    int N;
    int nthreads;
    int id;
    double total_time;
    double *A; // source matrix X
    double *B; // source matrix Y
    double *C; // dest matrix
};

// dynamic array of pointers to free.
#define INIT_TO_FREE_CAP 10
static void **to_free; // list of pointers to free
static int to_free_len;
static int to_free_cap = INIT_TO_FREE_CAP;

// static double total_time;

int main()
{

    sysctlbyname("hw.cachelinesize", &CLS, &len, NULL, 0); // here on MacOS

    printf("Cache Line Size: %lld\n", CLS);

    const int xrow = 100, xcol = 100;
    const int yrow = 100, ycol = 100;


    char *filename = "mat_mul.csv";
    FILE *fp = fopen(filename, "wb");
    if (!fp)
        exit(1);
    fputs(",nthreads,M,N,time\n",fp); // columns -- nthreads, M, N, time
    int idx = 0;


    struct {
        int M;
        int N;
    } dims[] = {
        100, 100,
        144, 144,
        200, 200,
        288, 288,
        400, 400,
        576, 576,
        800, 800,
        1024, 1024,
        1600, 1600
    };

    const int ndims = 9;

    pthread_t *thread_arr;  // cannot assign to non-static member [member] with const qualified type
    struct mat_arg *arg_arr;
    to_free = malloc(to_free_cap * sizeof(void *));
    if (!to_free)
        eprintf("Could not allocate to_free");
    thread_arr = xmalloc(ndims * sizeof(pthread_t));
    arg_arr = xmalloc(ndims * sizeof(struct mat_arg)); // TODO fix the logic here -- should be tied to the number of threads, not the number of dims.



    int nthreads[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int nthreads_len = 8;

    for (int i = 0; i < ndims; ++i) {
        // is it doing what i expect or is it writing through entries too early? I don't know! 
        // Are different threads working with the same entries ever?

        double *X_p = malloc(dims[i].M * dims[i].N * sizeof(double));
        double *Y_p = malloc(dims[i].N * dims[i].M * sizeof(double));
        identify(Y_p, dims[i].M); // assume square
        // set Y_p to identity matrix
        // assert that Z_p == X_p at al points.
        double *Z_p = malloc(dims[i].M * dims[i].M * sizeof(double));
        // const double (*X)[dims[i].M][dims[i].N] = (double (*)[dims[i].M][dims[i].N])X_p;
        // const double (*Y)[dims[i].N][dims[i].M] = (double (*)[dims[i].N][dims[i].M])Y_p;
        // double (*Z)[dims[i].M][dims[i].M]       = (double (*)[dims[i].M][dims[i].M])Z_p;
        // double X[dims[i].M][dims[i].N]; 
        // double Y[dims[i].N][dims[i].M]; 
        // double Z[dims[i].M][dims[i].M]; 

        printf("Dims: M = %d, N = %d\n", dims[i].M, dims[i].N);

        void *ret = NULL;

        for (int k = 0; k < nthreads_len; ++k) {
            double total_time = 0;
            printf("Nthreads: %d\n", nthreads[k]);
            fprintf(fp, "%d,%d,%d,%d,", idx++, nthreads[k], dims[i].M, dims[i].N);

            // total_time = 0;

            // #pragma omp parallel 
            // {
            //     double tstart = omp_get_wtime();

            //     // one 1/1000th of the outer iterations are one xrow value --
            //     // vast majority of the time each value is exclusively written to by one thread.
            //     // but what about the values that aren't?
            //     // make identity mamtrix to test.
            //     // #pragma omp parallel for num_threads(nthreads[k]) collapse(2)
            //     for (int xrow = 0; xrow < dims[i].M; ++xrow) {
            //         for (int l = 0; l < dims[i].N; ++l) {
            //             for (int ycol = 0; ycol < dims[i].M; ++ycol) {
            //                 (*Z)[xrow][ycol] += (*X)[xrow][l] * (*Y)[l][ycol];
            //             }
            //         }
            //     }
            //     double tend = omp_get_wtime();
            //     total_time = tend - tstart;
            // }

            // create pthreads
            for (int j = 0; j < nthreads[k]; ++j) {
                arg_arr[j].M = dims[i].M;
                arg_arr[j].N = dims[i].N;
                arg_arr[j].nthreads = nthreads[k];
                arg_arr[j].id = j;
                arg_arr[j].total_time = 0;
                arg_arr[j].A = X_p;
                arg_arr[j].B = Y_p;
                arg_arr[j].C = Z_p;
            }

            double tstart = omp_get_wtime();
            // create pthreads
            for (int j = 0; j < nthreads[k]; ++j) {
                pthread_create(&thread_arr[j], NULL, thread_fun, &arg_arr[j]);
            }

            for (int j = 0; j < nthreads[k]; ++j) {
                pthread_join(thread_arr[j], &ret);
                // total_time +=  arg_arr[j].total_time;
            }
            double tend = omp_get_wtime();
            total_time = tend - tstart;

            // join pthreads

            // for (int j = 0; j < nthreads[k]; ++j) {
            //     arg_arr[j].M = dims[i].M;
            //     arg_arr[j].N = dims[i].N;
            //     arg_arr[j].nthreads = nthreads[k];
            //     arg_arr[j].id = j;
            //     arg_arr[j].A = (double *)X;
            //     arg_arr[j].B = (double *)Y;
            //     arg_arr[j].C = (double *)Z;
            // }

            // for (int j = 0; j < nthreads[k]; ++j) {
            //     pthread_create(&thread_arr[j], NULL, thread_fun, &arg_arr[j]);
            // }

            // for (int j = 0; j < ndims; ++j) {
            //     pthread_join(thread_arr[j], NULL);
            // }


            printf("Total Time: %g\n", total_time);
            fprintf(fp, "%g\n", total_time);
        }


        free(X_p);
        free(Y_p);
        free(Z_p);

    }

    free(thread_arr);
    fflush(fp);
    fclose(fp);

    // free(arg_arr);


    // clean_up();

    // double (*X)[xrow][xcol] = (double (*)[xrow][xcol])get_random_matrix(xrow, xcol);
    // double (*Y)[yrow][ycol] = (double (*)[yrow][ycol])get_random_matrix(yrow, ycol);


    // double Z[2][2] = {0};

    // print_mat(2, 3, &X);
    // printf("\n");
    // print_mat(3, 2, &Y);
    // printf("\n");
    // printf("Z Before:\n");
    // print_mat(2, 2, &Z);
    // printf("\n");
    // double start = omp_get_wtime();
    // matrix_mul_ordinary(2, 3, &X, &Y, &Z);
    // double end = omp_get_wtime();

    // printf("Total time: %4gs", start - end);

    // printf("Z After:\n");
    // print_mat(2, 2, &Z);
    // printf("\n");
    return 0;
}

// input a square matrix X and side length len
// alter the matrix in place so that it is the identity matrix
void identify(double *X, int len)
{
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            *(X + len * i + j) = 1. * (i == j);
        }
    }

}

static inline void clean_up()
{
    while (to_free_len >= 0) {
        free(*to_free + --to_free_len);
    }
    free(to_free);
}

#define MAT_RANGE 128

// return a pointer to the first double in a newly allocated matrix
static inline double *get_random_matrix(const int row, const int col)
{
    double *ptr = xmalloc(row * col * sizeof(double)); // allocate a big matrix -- don't need to even initialize it, cause its full of garbage
    return ptr;
}

// malloc wrapper
// check for ENOMEM
// add allocated pointer to list of allocated and increment number of allocated.
static inline void *xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
        eprintf("Malloc failed!");
    // add pointer to to_free
    // check to_free_cap
    if (to_free_len == to_free_cap) {
        to_free_cap *= 2;
        to_free = xrealloc(to_free, to_free_cap);
    }
    to_free[to_free_len++] = ptr;
    return ptr;
}

static inline void *xrealloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    if (!ptr)
        eprintf("Realloc failed!");
    return ptr;
}

void *thread_fun(void *v)
{
    struct mat_arg *arg = (struct mat_arg *)v;
    const int M = arg->M,   N = arg->N;
    // const double (*X)[M][N] = (const double (*)[M][N])arg->A; // treat this as a podoubleer to a two diMensional array
    // const double (*Y)[M][N] = (const double (*)[N][M])arg->B; // treat this as a podoubleer to a two diMensional array
    // double (*Z)[M][M]       =       (double (*)[M][M])arg->C;
    double *X = arg->A;
    double *Y = arg->B;
    double *Z = arg->C;

    const int stride = arg->M * arg->N / arg->nthreads;

    const int i_start = arg->id * stride;
    const int i_stop = (arg->id < arg->nthreads - 1) ? i_start + stride : arg->M * arg->N;

    double tstart = omp_get_wtime();
    // matrix_mul_ordinary(M, N, i_start, i_stop, X, Y, Z);
    int xrow, i, ycol, ij;
    // for (xrow = 0; xrow < M; ++xrow) {
    // for (i = 0; i < N; ++i) {
    for (ij = /*0*/i_start; ij < /*M * N*/ i_stop; ++ij) { // collapsed loop
        xrow = ij / M; i = ij % N;
        for (ycol = 0; ycol < M; ++ycol) {
            // (*Z)[xrow][ycol] += (*X)[xrow][i] * (*Y)[i][ycol];
            *(Z + M * xrow + ycol) = *(X + M * xrow + i) * *(Y + N * i + ycol);
        }
    }

    double tend = omp_get_wtime();

    arg->total_time = tend - tstart;

    return NULL;
}

#define SM (CLS / sizeof(double))

// credit: Ulrich Drepper -- What Every Programmer Needs to know About Memory (2007).
static inline void mat_mul(const int M, const int N, const double (*restrict X)[M][N],
                                              const double (*restrict Y)[N][M],
                                                    double (*restrict Z)[M][M])
{
    int i, j, k, i2, j2, k2;
    const double *rX, *rY;
    double *rZ;

    for (i = 0; i < M; i += SM) {
        for (j = 0; j < M; j += SM) {
            for (k = 0; k < N; k += SM) {
                for (i2 = 0, rZ = &(*Z)[i][j], rX = &(*X)[i][k];
                    i2 < SM; ++i2, rZ += N, rX += N) {
                    for (k2 = 0, rY = &(*Y)[k][j];
                        k2 < SM; ++k2, rY += N) {
                        for (j2 = 0; j2 < SM; ++j2) {
                            rZ[j2] += rX[k2] * rY[j2];
                        }
                    }
                }
            }
        }
    }
}

// complexity O(M^2*N)! Yikes!!
static inline void
matrix_mul_ordinary(const int M, const int N,
                    const int i_start, const int i_stop,  // partitioning N among threads.
                    const double (*restrict X)[M][N],
                    const double (*restrict Y)[N][M],
                          double (*restrict Z)[M][M])
{
    int xrow, i, ycol, ij;
    // for (xrow = 0; xrow < M; ++xrow) {
        // for (i = 0; i < N; ++i) {
        for (ij = 0; ij < M * N; ++ij) { // collapsed loop
            xrow = ij / M; i = ij % N;
            for (ycol = 0; ycol < M; ++ycol) {
                (*Z)[xrow][ycol] += (*X)[xrow][i] * (*Y)[i][ycol];
            }
        }
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
