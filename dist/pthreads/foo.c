#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define MAX 10

/*error handling function: prints out error message*/
int print_error(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(2);
}

/* helper function: genRandomArray
 * fills an input array of specified length (length) with random
   values from 0 to MAX-1
*/
void genRandomArray(int *array, int length) {
    int i;
    for (i = 0; i < length; i++) {
        array[i] = rand() % MAX;
    }
}

struct t_arg {
    int id; //thread id
    int numthreads; //number of threads
    int *ap; //pointer to array
    int length;
    int *foundIndex; // Result
    int lookForValue;
};

/*parallel version of find elements.
*/
void *findElements(void *args) {
    //extract arguments
    struct t_arg *myargs = (struct t_arg *)args;
    int myid = myargs->id;
    int nthreads = myargs->numthreads;
    int length = myargs->length;
    int lookForValue = myargs->lookForValue;
    int *array = myargs->ap;
    int *foundIndex = myargs->foundIndex;

    //assign work to the thread
    int chunk = (length+nthreads) / nthreads; //nominal chunk size
    int start_i = myid * chunk;
    int end_i = (myid+1)*chunk - 1;

    if (end_i >= length)
        end_i = length-1;



    int i;
    //heart of the program (Finding element, return their index)
    for (i = start_i; i <= end_i; i++) {
        if(array[i] == lookForValue) { // Found
            *foundIndex = i;
            printf("[Thread %d] start_i = %d, end_i = %d. Found Value %d at index %d\n", myid, start_i, end_i, lookForValue, *foundIndex);
            return NULL;
        }
    }
    printf("[Thread %d] start_i = %d, end_i = %d. Found nothing!\n", myid, start_i, end_i);

    return NULL;
}


int main(int argc, char **argv) {

    if (argc != 5) {
        fprintf(stderr, "usage: %s <n> <p?> <t>\n", argv[0]);
        fprintf(stderr, "where <n> is the length of the array\n");
        fprintf(stderr, "and <p?> is the print option (0/1)\n");
        fprintf(stderr, "and <t> is the number of threads\n");
        fprintf(stderr, "and <x> is the value in the array you are looking for\n");
        return 1;
    }

    srand(10);

    int t;
    int length = strtol(argv[1], NULL, 10);
    int verbose = atoi(argv[2]);
    int nthreads = strtol(argv[3], NULL, 10);
    int lookForValue = atoi(argv[4]);
    if (nthreads < 1) print_error("ERROR: need a positive number of threads");
    if (length < nthreads) print_error("ERROR: length must be greater than nthreads");
    int ret; //for error checking

    //generate random array of elements of specified length
    int *array = malloc(length * sizeof(int));
    if (!array) print_error("ERROR: malloc failed");

    genRandomArray(array, length);
    if(verbose == 1) {
        printf("Input array: ");
        for(int h = 0; h < length; ++h) // Print the input array.
            printf("%d ", array[h]);
        printf("\n\n");
    }

    //specify counts array
    int counts[MAX] = {0};

    //allocate threads and args array
    pthread_t *thread_array; //pointer to future thread array
    thread_array = malloc(nthreads * sizeof(pthread_t)); //allocate the array
    struct t_arg *thread_args = malloc(nthreads * sizeof(struct t_arg));
    if (!thread_array || !thread_args) print_error("ERROR: malloc failed");

  //fill thread array with parameters
    for (t = 0; t < nthreads; t++) {
        int foundIndex;
        thread_args[t].id = t;
        thread_args[t].numthreads = nthreads;
        thread_args[t].ap = array; //pointer to array
        thread_args[t].length = length;
        thread_args[t].lookForValue = lookForValue;
        thread_args[t].foundIndex = &foundIndex;
    }

    for (t = 0; t < nthreads; t++) {
        ret = pthread_create( &thread_array[t], NULL, findElements, &thread_args[t]);
        if (ret) print_error("ERROR: pthread_create failed");
    }

    for (t = 0; t < nthreads; t++) {
        ret = pthread_join(thread_array[t], NULL);
        if (ret) print_error("ERROR: pthread_create failed");
    }


    free(thread_array);
    free(array);

    return 0;
}
