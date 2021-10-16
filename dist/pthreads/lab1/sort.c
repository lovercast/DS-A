#include <stdio.h>
#include <stdlib.h> /* malloc, realloc, strtol */
#include <pthread.h>
#include <unistd.h> /* getopt */
#include <string.h> /* strncpy */
#include <stdarg.h> /* va_list */
#include <math.h> /* sqrt */
// #include <assert.h> /* debugging */

#define BUF_MAX 20
#define THREAD_MAX 10000
#define panic(s, a) (fprintf(stderr, "Panic: " s "\n", (a)), exit(1))
#define check_input(b, s, a) if ((b)) panic(s, (a))

void *thread_search(void *);                   /* thread function */
void gen_random(int[], int, int);              /* populate random array */
void printcolor_arr(int[], int, int);
void print_arr(int[], int, int);
void *xmalloc(size_t);                         /* malloc wrapper */
void *xrealloc(void *, size_t);
void value_found(int);
void overwrite_arr(int *, int *, int, int);    /* write data to new arr */
void print_scores(int, int *);
char *get_color(int);
void parse_opt(int);
void usage();

#define printcolorf(id, fmt, ...)\
do {\
    printf("%s", get_color(id));\
    printf(fmt, __VA_ARGS__);\
    printf("\033[0m");\
}\
while (0)

/* our lovely custom struct */
struct t_arg {
    int id;                  /* why long? do we need 32 more bits of threads? */
    int nthreads;
    int *arrp;
    int *scoresp;
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
char *optstring = "hv";

int *foundp;
size_t foundp_size;
int fidx = 0;

char *colorp;

/* Search an array of random integers */
int main(int argc, char *argv[])
{
    int i;
    int arrlen, abclen, nthreads, value; /* values from input */
    char buf[BUF_MAX];
    pthread_t *tp;
    struct t_arg *argp;
    int *arrp, /**countp,*/ *scoresp; /* pointers to arrays in memory. */

    while (getopt(argc, argv, optstring) != -1)
        parse_opt(optopt);

    if (opt & HELP)
        usage();

    if (opt == 0 && argc < 4 || opt != 0 && argc < 5)
        fprintf(stderr, "\nMissing required arguments \n"), usage();


    /* strtol() is recommended over atoi() for new code --
     * you learn something new everyday.   */
    check_input((arrlen = strtol(opt ? argv[2] : argv[1], NULL, 10)) < 0,
                "Invalid arrlen: %d", arrlen);
    check_input((abclen = strtol(opt ? argv[3] : argv[2], NULL, 10)) < 0,
                "Invalid abclen: %d", abclen);
    check_input((nthreads = strtol(opt ? argv[4] : argv[3], NULL, 10)) < 0 || nthreads > THREAD_MAX,
                "Invalid nthreads: %d", nthreads);

    /* read value from keyboard */
    printf("\nPlease enter the value to search for (< %d): ", abclen);
    fgets(buf, BUF_MAX, stdin);

    check_input((value = strtol(buf, NULL, 10)) < 0 || value >= abclen,
                "Invalid value: %d", value);

    foundp_size = 2 * arrlen/abclen * sizeof(int);

    /* allocate */
    tp      = xmalloc(nthreads * sizeof(pthread_t));
    argp    = xmalloc(nthreads * sizeof(struct t_arg));
    arrp    = xmalloc(arrlen   * sizeof(int));
    scoresp = xmalloc(nthreads * sizeof(int));
    foundp  = xmalloc(foundp_size);

    /* populate array with "randoms" */
    srand(42);
    gen_random(arrp, arrlen, abclen);

    if (opt & VERBOSE) { /* print */
        printf("\nHere is your perfect array.\n");
        printcolor_arr(arrp, arrlen, nthreads);
        printf("\n");
    }

    /* create threads */
    for (i = 0; i < nthreads; i++) {
        argp[i].id = i;
        argp[i].nthreads = nthreads;
        argp[i].arrp = arrp;
        // argp[i].countp = countp;
        argp[i].scoresp = scoresp;
        argp[i].arrlen = arrlen;
        argp[i].abclen = abclen;
        argp[i].value = value;
        if(pthread_create(&tp[i], NULL, thread_search, &argp[i])) /* return non zero on error */
            errorf("pthread_create failed with id %d\n", i);
    }

    /* What can we do between the create and the join???  */
    /* Write witty comments! */

    /* join threads */
    for (i = 0; i < nthreads; i++) {
        if(pthread_join(tp[i], NULL))
            errorf("pthread_join failed with id %d\n", i);
    }

    printf("scores:\n");
    print_scores(nthreads, scoresp);
    printf("\n");

    if (opt & VERBOSE) { /* print */
        printf("Indices of value:\n");
        print_arr(foundp, fidx, (int)sqrt(fidx));
        printf("\n");
    }

    /* clean up */
    free(tp),        free(argp),        free(foundp);
    free(arrp),      free(scoresp);



}

void gen_random(int arr[], int arrlen, int abclen)
{
    int i;

    for (i = 0; i < arrlen; i++)
        arr[i] = rand() % abclen;
}


void *thread_search(void *v)
{
    int i;
    struct t_arg *arg = (struct t_arg *)v;
    int stride = arg->arrlen/arg->nthreads;
    int start_pos = arg->id * stride;
    int end_pos = (arg->id < arg->nthreads-1) ? start_pos + stride : arg->arrlen;
    /* count elements in array slice and write to countp */
    for (i = start_pos; i < end_pos; i++) {
        if (arg->arrp[i] == arg->value) {
            value_found(i);
            arg->scoresp[arg->id]++; /* Yay! It scored! */
        }
        // num = arg->arrp[i];
        // arg->countp[num]++;
    }
    return (void *)"😀";
}

/* the value was found at what idx */
void value_found(int idx)
{
    foundp[fidx++] = idx;
    if (fidx == (int)foundp_size-1) {
        foundp_size *= 2;
        foundp = xrealloc(foundp, foundp_size);
    }
}

void *xmalloc(size_t size)
{
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr, "Malloc failed!");
        exit(1);
    }
    return p;
}

void *xrealloc(void *p, size_t size)
{
    void *p_new = realloc(p, size);
    if (!p_new) {
        fprintf(stderr, "Realloc failed!");
        exit(1);
    }
    return p;
}

/* ignore -- from the count_sort problem */
// void overwrite_arr(int *arrp, int *countp, int arrlen, int abclen)
// {
    // int i = 0, a, sum = 0;

    // for (a = 0; a < abclen; a++) {
        // for (i = 0; i < countp[a]; i+=1, sum+=1) {
            // if (sum == arrlen)
                // goto fail;
            // arrp[sum] = a;
        // }
    // }
    // return;

// fail:
    // perror("Something went wrong with your array length!");
    // exit(1);
// }

char *get_color(int id)
{

    switch(id % 6) {
    case 0:
        return "\033[2;30;101m";
    case 1:
        return "\033[2;30;102m";
    case 2:
        return "\033[2;30;103m";
    case 3:
        return "\033[2;30;104m";
    case 4:
        return "\033[2;30;105m";
    case 5:
        return "\033[2;30;106m";
    }
    return NULL;
}

void print_scores(int nthreads, int *scoresp)
{
    for (int i = 0; i < nthreads; i++) {
        printf("[");
        printcolorf(i, "Thread %2d", i);
        printf("]\t%d\n", scoresp[i]);
    }
}

void print_arr(int arr[], int arrlen, int width)
{
    printf("[");
    for (int i = 0; i < arrlen; i++) {
        if (i % width == 0 && i != 0)
            printf("[");
        printf("%3d ", arr[i]);
        if (i % width == width-1 && i != arrlen-1)
            printf("]\n");
    }
    printf("]\n");

}

void printcolor_arr(int arr[], int arrlen, int nthreads)
{
    int stride = arrlen/nthreads;
    printf("[");
    for (int i = 0; i < arrlen; i++) {
        if (i % 20 == 0 && i != 0)
            printf("[");
        printcolorf(i/stride, "%3d ", arr[i]);
        if (i % 20 == 19 && i != arrlen-1)
            printf("]\n");
    }
    printf("]\n");
}

void parse_opt(int c)
{
    switch(c) {
    case 'h':
        opt |= HELP;
        break;
    case 'v':
        opt |= VERBOSE;
        break;
    default:
        fprintf(stderr, "Bad help flag: %c\n", c);
        usage();
    }
}

void usage()
{
    fprintf(stderr, "\nEXAMPLE:\
\n\tcc foo.c -o foo\
\n\t./foo -v 500 20 19\
\n\t./foo 100000000 1000 12 (don't use -v)\
\nUSAGE:\
\n\t./foo [OPTIONS] ARRLEN ABCLEN NTHREADS\
\n\tARRLEN --------------- length of the array to sort,\
\n\tABCLEN --------------- length of the list of numbers to be sorted,\
\n\tNTHREADS ------------- the number of threads,\
\nOPTIONS:\
\n\t-h  ------------------ help   (print this message)\
\n\t-v  ------------------ verbose   (print the arrays)\
            \n\n");

    exit(1);
}

