#include <math.h> /* sqrt */
#include <pthread.h>
#include <stdarg.h> /* va_list */
#include <stdio.h>
#include <stdlib.h> /* malloc, realloc, strtol */
#include <string.h> /* strncpy */
#include <sys/time.h> /* gettimeofday */
#include <unistd.h> /* getopt */
// #include <assert.h> /* debugging */

#define BUF_MAX 20
#define THREAD_MAX 10000
#define errorf(s, ...) (fprintf(stderr, "Error: " s "\n", __VA_ARGS__), exit(1))
#define check_input(b, s, ...) \
    if ((b))                   \
    errorf(s, __VA_ARGS__)

void* thread_search(void*); /* thread function */
void gen_random(int[], int, int); /* populate random array */
void printcolor_arr(int[], int, int);
void print_arr(int[], int, int);
void* xmalloc(size_t); /* malloc wrapper */
void* xrealloc(void*, size_t);
void value_found(int);
void overwrite_arr(int*, int*, int, int); /* write data to new arr */
void print_scores(int, int*, time_t*);
char* get_color(int);
void parse_opt(int);
void usage();

#define printcolorf(id, fmt, ...)    \
    do {                             \
        printf("%s", get_color(id)); \
        printf(fmt, __VA_ARGS__);    \
        printf("\033[0m");           \
    } while (0)

/* our lovely custom struct */
struct t_arg {
    int id; /* why long? do we need 32 more bits of threads? */
    int nthreads;
    int* arrp;
    int* scoresp;
    time_t* timep; // record how long each thread takes.
    int arrlen;
    int abclen;
    int value;
};

/* options */
enum options {
    HELP = 1,
    VERBOSE = 2
};

/* option flag */
int opt = 0;
char* optstring = "hv";

int* foundp;
size_t foundp_size;
int fidx = 0;

char* colorp;

/* global mutex */
pthread_mutex_t m;