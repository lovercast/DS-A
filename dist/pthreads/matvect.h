
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <sys/sysctl.h>
#include <omp.h>

struct mat_arg { // 56 bytes! Fits in one cache line fortunately
    int N;
    int id;
    int row0;
    int row1;
    double thread_time;  // thread will write how much time it took here.
    double rows;    // how many pieces of the product vector is it responsible for
    double *A; // source matrix X
    double *y; // output vector piece
    double *x; // a piece of heap memory that the thread will write its answers on.
};

// keep track of memory with a linked list
struct llist {
    void *ptr; // a pointer to memory to free
    struct llist *next;
};

#define eprintf(...) (fprintf(stderr, __VA_ARGS__), exit(1))

/* Function prototypes */
double *get_random_matrix(const int row, const int col);
void print_mat(const int m, const int n, const double (*X)[m][n]);
void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
void *thread_fun(void *v);
void clean_up();
void identify(double *X, int len);
void traverse_free(struct llist *self);
void print_vect(int N, double *a);



static struct llist *head = NULL;
static struct llist *current = NULL;