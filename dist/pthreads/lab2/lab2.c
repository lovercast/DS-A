#include "lab2.h"

// Description --
// Multiply a vector X by a scalar a in parallel.
// Command line arguments:
// options -
//      -h      print usage message
//      -v      print the arrays before and after
//      -x      print the data in a machine-readable way, incompatible with -v. 
//              Takes a command line argument for the scalar value following the -x flag.
// nthreads
// length of array

// if -x is unselected, then the program accepts an input from stdin for the value of
//          the scalar to multiply the random array X by.


// version 2
#ifndef ___UNSAFE___
#define ___UNSAFE___
#endif

int main(const int argc, const char *argv[])
{
    int i;
    long a, nthreads, arrlen;
    long *X;
    pthread_t *threads;
    struct t_arg *args;
    double *timep;

    // double start = omp_get_wtime();
    //double end = omp_get_wtime();

    // printf("%g\n",end - start);


    int opts;
    opts = parse_opt(argc, argv);

    // printf("%d", opts);

    if (opts & HELP)
        usage();

    if (argc < 3 + (opts != 0))
        eprintf("Missing required arguments. Have %d. Need %d.", argc, 3 + (opts != 0));

    nthreads = strtol(argv[1 + (opts != 0) + ((opts & EXPORT) != 0)], NULL, BUFMAX);
    arrlen = strtol(argv[2 + (opts != 0) + ((opts & EXPORT) != 0)], NULL, BUFMAX);

    /* allocate */
    X = xmalloc(arrlen * sizeof(long));
    threads = xmalloc(nthreads * sizeof(pthread_t));
    args = xmalloc(nthreads * sizeof(struct t_arg));
    timep = xmalloc(nthreads * sizeof(long));

    if (!(opts & EXPORT)) {
        printf("Please enter a scalar: ");
        scanf("%ld", &a);
        printf("\n");
    } else {
        a = strtol(argv[2], NULL, BUFMAX);
    }

    // printf("a %ld nthreads %ld arrlen %ld\n", a, nthreads, arrlen);

    // return 0;

    #ifndef ___UNSAFE___
    pthread_mutex_init(&m, NULL);
    #endif

/* populate array with "randoms" */
#ifdef ___DEBUG___
    srand(42);
#else
    srand(time(NULL));
#endif
    gen_random(X, arrlen, LAB2_LIM);

    if (opts & VERBOSE) { /* print */
        printf("\nArray before:\n");
        print_arr(X, arrlen, 20); /* decide width here */
    }

    /* pthread_create */
    for (i = 0; i < nthreads; ++i) {
        // assign attributes and join
        args[i].id = i;
        args[i].nthreads = nthreads;
        args[i].arrlen = arrlen;
        args[i].X = X;
        args[i].timep = timep;
        args[i].a = a;
    }

    double tstart = omp_get_wtime();

    for (i = 0; i < nthreads; ++i) {
        pthread_create(&threads[i], NULL, scalar, &args[i]);
    }

    /* pthread_join */
    for (i = 0; i < nthreads; ++i) {
        pthread_join(threads[i], NULL);
    }
    double tend = omp_get_wtime();

    /* clean up */
    #ifndef ___UNSAFE___
    pthread_mutex_destroy(&m);
    #endif

    if (opts & VERBOSE) { /* print */
        printf("\nArray after:\n");
        print_arr(X, arrlen, 20); /* decide width here */
    }

    print_times(nthreads, arrlen, tend - tstart, timep, opts);


    // TODO print threads times.
    // TODO print average time.

    free(X);
    free(threads);
    free(args);
    free(timep);
    return 0;
}


void* xmalloc(const size_t size)
{
    void* p = malloc(size);
    if (!p) {
        fprintf(stderr, "Malloc failed!");
        exit(1);
    }
    return p;
}

/*
 * Parse the options
 * if any, and switch
 */
int parse_opt(const int argc, const char *restrict argv[])
{
    int i, j;
    int opt = 0;
    if (argc < 2)
        return 0;
    else if (argv[1][0] != '-')
        return 0;

    char c;
    for (i = 1; (c = argv[1][i]) != '\0'; ++i) {
        // match against optstr
        for (j = 0; optstr[j] != '\0'; j++) {
            if (optstr[j] == c)
                break;
        }
        if (optstr[j] != c)
            eprintf("Unknown option: -%c", c);

        switch (c) {
        case 'h':
            opt |= HELP;
            break;
        case 'v':
            opt |= VERBOSE;
            break;
        case 'x':
            opt |= EXPORT;
            break;
        }
    }
    if (opt & VERBOSE && opt & EXPORT)
        usage();
    return opt;
}

// thread function
// scalar multiplication of an array
void *scalar(void *v)
{
    int i;
    int stride, start, end;
    struct t_arg* arg = (struct t_arg*)v;
    // struct timeval tstart, tend; // does declaring a struct allocate memory on stack?
    double tstart, tend;

    // gettimeofday(&tstart, NULL); // or does allocation for the struct happen here?

    tstart = omp_get_wtime();

    // calculate the first and last indices for me to calculate
    stride = arg->arrlen / arg->nthreads + (arg->arrlen % arg->nthreads != 0); // ((arg->arrlen % arg->nthreads) & 1L); // round up
    start = arg->id * stride;
    end = (arg->id < arg->nthreads - 1) ? start + stride : arg->arrlen;

    if (end - start <= 0 || end > arg->arrlen)
        return NULL;

    /* copy memory from the given array to a local copy */
    // long copy[end - start]; // busting the stack with data in the 8MB range -- have to allocate this

#ifndef ___UNSAFE___
    long *copy = malloc(sizeof(long)*(end-start));

    // pthread_mutex_lock(&m);
    memcpy(copy, &arg->X[start], sizeof(long)*(end-start));

    // pthread_mutex_unlock(&m);

    for (i = 0; i < end - start; ++i)
        copy[i] *= arg->a;

    /* mutex lock when writing to shared data */
    pthread_mutex_lock(&m);
#else 
    for (i = start; i < end; ++i) {
        arg->X[i] *= arg->a;
    }
#endif



#ifdef ___DEBUG___
    print_arr(copy, end - start, 20); /* debug */
#endif


#ifndef ___UNSAFE___
    memcpy(&arg->X[start], copy, (end - start) * sizeof(long));
    pthread_mutex_unlock(&m);
#endif

    // gettimeofday(&tend, NULL);
    // suseconds_t totaltime = (tend.tv_sec - tstart.tv_sec) * 1e6 + tend.tv_usec - tstart.tv_usec;
    tend = omp_get_wtime();
    
#ifndef ___UNSAFE___
    pthread_mutex_lock(&m);
#endif
    arg->timep[arg->id] = tend - tstart;
#ifndef ___UNSAFE___
    pthread_mutex_unlock(&m);
    free(copy);
#endif
    return NULL;
}

void gen_random(long arr[], const int arrlen, const int mod)
{
    for (int i = 0; i < arrlen; i++)
        arr[i] = rand() % mod;
}

void print_arr(const long *restrict arr, const int arrlen, const int width)
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

// what do I need to do with the output for python to be able to process it?
void print_times(const long nthreads, const long arrlen, double total_time, double *restrict timep, const int opts)
{
    suseconds_t sum;
    struct stats s;
    get_stats(&s, nthreads, timep);
    if (opts & VERBOSE) {
        printf("\nTimes (s):\n");
    } 
    if (opts & VERBOSE) {
        for (int i = 0; i < nthreads; ++i) 
            printf("Thread %d:\t%g s\n", i, timep[i]);
        printf("Total: %8.4g\n", s.sum);
        printf("Mean: %8.4g\n", s.mean);
        printf("Median: %8.4g\n", s.med);
        printf("Stdev: %8.4g\n", s.stdev);
    } else if (opts & EXPORT) {
        fprintf(stdout, "%ld ", nthreads);
        fprintf(stdout, "%ld ", arrlen);
        // fprintf(stdout, "%g ", s.sum);
        fprintf(stdout, "%g ", total_time);
        fprintf(stdout, "%g ", s.mean);
        fprintf(stdout, "%g ", s.med);
        fprintf(stdout, "%g ", s.stdev);
        fprintf(stdout, "\n");
    }
}


// Fill a stats struct with stats (mean, median, stdev).
void get_stats(struct stats *restrict s, const long len, double *restrict timep)
{
    int i;
    // initialize your variables!!!!! Automatic storage class variables contain garbage! I learn this anew every day.
    double sum = 0, sumsq = 0, mean = 0, med = 0, stdev = 0;
    
    for (i = 0; i < len; i++) {
        sum += timep[i];
        sumsq += timep[i] * timep[i]; // what if this overflows? It shouldn't -- 
    }
    mean = sum/len;
    stdev = sumsq/len - mean * mean;
    stdev = sqrt(stdev);

    insert_sort(timep, len);
    med = timep[len/2];
    if (len % 2 == 0) {
        med += timep[len/2 - 1];
        med /= 2;
    }

    s->sum = sum;
    s->mean = mean;
    s->med = med;
    s->stdev = stdev;
}

// Sort to get the median value.
// Acceptable complexity of O(n^2) for small values of n 
void insert_sort(double *restrict timep, const int len)
{
    int i, j;
    double tem;
    for (i = 0; i < len; i++) {
        j = i;
        while (timep[j] < timep[j-1] && j > 0) {
            tem = timep[j-1];
            timep[j-1] = timep[j];
            timep[j] = tem;
            --j;
        }
    }
}


void usage()
{
    fprintf(stderr, "\n\
BUILD COMMAND:\n\
\tcc lab2.c -o lab2 -lpthread\n\
USAGE:\n\
\t./lab2 [OPTIONS] NTHREADS ARRLEN\n\
\t./lab2 -v NTHREADS ARRLEN {to see the arrays printed}\n\
\t./lab2 -x [scalar value] NTHREADS ARRLEN {to export the data}\n\
\t./lab2 -h {to print this message}\n\
EXAMPLE:\n\
\t./lab2 -v 4 100 {enter 2 when prompted}\n\
PARAMETERS:\n\
\tNTHREADS ------------- the number of threads,\n\
\tARRLEN --------------- length of the array to sort,\n\
OPTIONS:\n\
\t-h  ------------------ help   (print this message)\n\
\t-v  ------------------ verbose   (print the arrays).\n\
\t-x  ------------------ export  (export data in machine readable form.\n\
                                        Cannot be used with verbose.)\n\
                                        Takes a command line argument for scalar instead of from stdin.\n\
\n\n");

    exit(1);
}
#ifdef ___UNSAFE___
#undef ___UNSAFE___
#endif