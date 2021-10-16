/* Second iteration of this program --
 * ========= Changelog ==========
 * include mutexes, timing,
 * include suggestions of classmates.
 * refactor panic to errorf -- panic is a confusing name.
 * move clutter to a header file -- program is more readable with main at top.
 * Generalize errorf and check_input to accept variadic args.
 * Change random seed to non-deterministic when not in debug mode.
 * Delete unused function count_sort from the other problem.
 * Change usage function to be more readable.
 * Add line numbers to array plot,
 * Add time of execution for each thread with gettimeofday.
 */
#include "thread_search.h"

/* Search an array of random integers for value from stdin. */
int main(int argc, char* argv[])
{
    int i;
    int arrlen, abclen, nthreads, value; /* values from input */
    char buf[BUF_MAX];
    pthread_t* tp;
    struct t_arg* argp;
    int *arrp, *scoresp; /* pointers to arrays in memory. */
    time_t* timep; // pointer to time array

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

    foundp_size = 2 * arrlen / abclen * sizeof(int);

    /* allocate */
    tp      = xmalloc(nthreads  * sizeof(pthread_t));
    argp    = xmalloc(nthreads  * sizeof(struct t_arg));
    arrp    = xmalloc(arrlen    * sizeof(int));
    scoresp = xmalloc(nthreads  * sizeof(int));
    foundp  = xmalloc(foundp_size);
    timep   = xmalloc(nthreads  * sizeof(time_t));

    /* initialize mutex */
    pthread_mutex_init(&m, NULL);

/* populate array with "randoms" */
#ifdef ___DEBUG___
    srand(42);
#else
    srand(time(NULL));
#endif
    gen_random(arrp, arrlen, abclen);

    if (opt & VERBOSE) { /* print */
        printf("\nHere is your perfect array.\n");
        printcolor_arr(arrp, arrlen, nthreads);
        printf("\n");
    }

    /* create threads */
    for (i = 0; i < nthreads; i++) {
        argp[i].id          = i;
        argp[i].nthreads    = nthreads;
        argp[i].arrp        = arrp;
        argp[i].scoresp     = scoresp;
        argp[i].timep       = timep;
        argp[i].arrlen      = arrlen;
        argp[i].abclen      = abclen;
        argp[i].value       = value;
        if (pthread_create(&tp[i], NULL, thread_search, &argp[i])) /* return non zero on error */
            errorf("pthread_create failed with id %d\n", i);
    }

    /* What can we do between the create and the join???  */
    /* Write witty comments! */

    /* join threads */
    for (i = 0; i < nthreads; i++) {
        if (pthread_join(tp[i], NULL))
            errorf("pthread_join failed with id %d\n", i);
    }

    printf("Scores: How many times each thread saw {%3d}\n", value);
    print_scores(nthreads, scoresp, timep);
    printf("\n");

    if (opt & VERBOSE) { /* print */
        printf("Indices of value {%3d}:\n", value);
        print_arr(foundp, fidx, (int)sqrt(fidx));
        printf("\n");
    }

    /* clean up */
    free(tp), free(argp), free(foundp);
    free(arrp), free(scoresp), free(timep);

    /* destroy mutex */
    pthread_mutex_destroy(&m);
}

void gen_random(int arr[], int arrlen, int abclen)
{
    int i;

    for (i = 0; i < arrlen; i++)
        arr[i] = rand() % abclen;
}

/* Thread function */
void* thread_search(void* v)
{
    int i;
    struct t_arg* arg = (struct t_arg*)v;
    time_t time_tot;
    struct timeval tstart, tend;
    if (gettimeofday(&tstart, NULL) != 0)
        errorf("Time failed: thread id: %d\n", arg->id);

    int stride      = arg->arrlen / arg->nthreads;
    int start_pos   = arg->id * stride;
    int end_pos     = (arg->id < arg->nthreads - 1) 
                    ? start_pos + stride 
                    : arg->arrlen;
    int score       = 0;
    /* count elements in array slice and write to countp */
    for (i = start_pos; i < end_pos; i++) {
        if (arg->arrp[i] == arg->value) {
            pthread_mutex_lock(&m);
            value_found(i); // no thread should be doing this at the same time.
            pthread_mutex_unlock(&m);
            ++score; /* Yay! It scored! */
        }
    }
    // lock this stuff --
    pthread_mutex_lock(&m); // no argument NULL here
    arg->scoresp[arg->id] = score;
    pthread_mutex_unlock(&m);

    /* record time it took */
    if (gettimeofday(&tend, NULL) != 0)
        errorf("Time failed: thread id: %d\n", arg->id);
    time_tot = (tend.tv_sec - tstart.tv_sec) * 1e6 + (tend.tv_usec - tstart.tv_usec);
    arg->timep[arg->id] = time_tot;
    return (void*)"😀";
}

/* the value was found at what idx */
/* The threads will enter this function -- howto make thread-safe? */
void value_found(int idx)
{
    foundp[fidx++] = idx;
    if (fidx == (int)foundp_size - 1) {
        foundp_size *= 2;
        foundp = xrealloc(foundp, foundp_size);
    }
}

void* xmalloc(size_t size)
{
    void* p = malloc(size);
    if (!p) {
        fprintf(stderr, "Malloc failed!");
        exit(1);
    }
    return p;
}

void* xrealloc(void* p, size_t size)
{
    void* p_new = realloc(p, size);
    if (!p_new) {
        fprintf(stderr, "Realloc failed!");
        exit(1);
    }
    return p;
}

char* get_color(int id)
{

    switch (id % 6) {
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

void print_scores(int nthreads, int* scoresp, time_t* timep)
{
    printf("Threads\t\tScore\t Time (µs)\n");
    for (int i = 0; i < nthreads; i++) {
        printf("[");
        printcolorf(i, "Thread %2d", i);
        printf("]\t%5d\t\t%2ld\n", scoresp[i], timep[i]);
    }
}

void print_arr(int arr[], int arrlen, int width)
{
    printf("[");
    for (int i = 0; i < arrlen; i++) {
        if (i % width == 0 && i != 0)
            printf("[");
        printf("%3d ", arr[i]);
        if (i % width == width - 1 && i != arrlen - 1)
            printf("]\n");
    }
    printf("]\n");
}

void printcolor_arr(int arr[], int arrlen, int nthreads)
{
    int stride = arrlen / nthreads;
    printf("  0 [");
    int width = 20;
    for (int i = 0; i < arrlen; i++) {
        if (i % width == 0 && i != 0)
            printf("%3d [", i);
        printcolorf(i / stride, "%3d ", arr[i]);
        if (i % width == width - 1 && i != arrlen - 1)
            printf("] %3d\n", i);
    }
    printf("] %3d\n", arrlen - 1);
}

void parse_opt(int c)
{
    switch (c) {
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
    fprintf(stderr, "\n\
EXAMPLE:\
\tcc foo.c -o foo\n\
\t./foo -v 500 20 19\n\
\t./foo 100000000 1000 12 (don't use -v)\n\
USAGE:\n\
\t./foo [OPTIONS] ARRLEN ABCLEN NTHREADS\n\
\tARRLEN --------------- length of the array to sort,\n\
\tABCLEN --------------- length of the list of numbers to be sorted,\n\
\tNTHREADS ------------- the number of threads,\n\
OPTIONS:\n\
\t-h  ------------------ help   (print this message)\n\
\t-v  ------------------ verbose   (print the arrays)\n\
            \n\n");

    exit(1);
}