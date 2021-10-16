#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memcpy */
#include <sys/time.h> /* gettimeofday */
#include <math.h>
// #include <stdarg.h>
#include <omp.h>

// #ifndef ___DEBUG___
// #define ___DEBUG___
// #endif
#ifdef ___DEBUG___
#undef ___DEBUG___
#endif
#define LAB2_LIM 1024


struct t_arg {
    long id;
    long nthreads;
    long arrlen;
    long *X;
    double *timep;
    long a;
};

struct stats {
    double sum;
    double mean;
    double med;
    double stdev;
};

int parse_opt(const int argc, const char *restrict *argv);
void* scalar(void* arg);
void gen_random(long arr[], int const arrlen, int const mod);
void print_arr(const long *restrict arr, const int arrlen, const int width);
void print_times(const long nthreads, const long arrlen, double total_time, double *restrict timep, const int opts);
void get_stats(struct stats *restrict s, const long len, double *restrict timep);
void insert_sort(double *restrict timep, const int len);
void* xmalloc(const size_t size);
void usage(void);


enum opts {
    HELP = 1,     /* print usage and quit */
    VERBOSE = 2,  /* print all data in the console */
    EXPORT = 4    /* export all data to stdout in machine readable form. Cannot be used with verbose. */
};

static const char* optstr = "hvx";

#define eprintf(...) (fprintf(stderr, __VA_ARGS__), exit(1))
#define BUFMAX 10
// how would you write this as an inline function?

pthread_mutex_t m;
