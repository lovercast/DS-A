// hello_pthreads.cpp : This file contains the 'main' function. Program execution begins and ends there.
// I don't know how to decompos the array into appropriate-sized pieces for each pthread

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // getting error: 'timespec': 'struct' type redefinition

// create an array - manually set length because I couldn't get command line args to work
const char* lengthChar = "100";
int MAX = 100;
int my_array[100];
int counts[100] = { 0 }; // here !!

// set number of threads - manually b/c couldn't get command line args to work
const char* n = "4";

// create struct
struct t_arg {
    long id; //thread id
    long numthreads; //number of threads
    int* ap; //pointer to array to be sorted
    int length; //length of  array to be sorted
    long* countp; //pointer to count array
};

void *countElms(void *args) { // added *
    //extract arguments
    struct t_arg* myargs = (struct t_arg*)args;
    long myid = myargs->id;
    long nthreads = myargs->numthreads;
    long length = myargs->length;
    int* array = myargs->ap;
    long* counts = myargs->countp;

    //assign work to the thread
    long chunk = length / nthreads; //nominal chunk size
    long start = myid * chunk;
    long end = (myid + 1) * chunk;
    long val;
    if (myid == nthreads - 1) {
        end = length;
    }

    // count the frequency of elements in my_array
    long i;
    for (i = start; i < end; i++) {
        val = my_array[i]; //read the value at index i
        counts[val] = counts[val] + 1; //update corresponding location in counts
    }

    return NULL;
}

void writeArray() {
    // overwrite (sort) my_array
    int i, j = 0, amt;
    for (i = 0; i < MAX; i++) {
        amt = counts[i]; //capture frequency of element i
        while (amt > 0) { //while all values aren't written
            my_array[j] = i; //replace value at index j of array_A with i
            j++; //go to next position in array_A
            amt--; //decrease the amount written by 1
        }
    }
}

void genRandArray() {
    for (int f = 0; f < 100; f++) {
        my_array[f] = rand() % +MAX;
    }
}

int main() {
    long t;
    int nthreads; //number of threads
    pthread_t *thread_array; //pointer to future thread array
    long* thread_ids;
    long length = strtol(lengthChar, NULL, 10);
    
    //generate random array of elements of specified length
    int* array = (int *) malloc(length * sizeof(int));

    // fill the array with random numbers between 0 and 100
    genRandArray();

    
    // strtol creates a long int value from n (with base 10 in this case)
    nthreads = strtol(n, NULL, 10);

    // Allocate space for thread structs and identifiers.
    thread_array = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
    thread_ids = (long *) malloc(nthreads * sizeof(long));
    struct t_arg* thread_args = (struct t_arg *) malloc(nthreads * sizeof(struct t_arg));

    //fill thread array with parameters
    for (t = 0; t < nthreads; t++) {
        thread_args[t].id = t;
        thread_args[t].numthreads = nthreads;
        thread_args[t].ap = array; //pointer to array
        thread_args[t].length = length;
        thread_args[t].countp = (long *)counts; //pointer to counts array // HERE
    }

    // Assign each thread an ID and create all the threads.
    for (t = 0; t < nthreads; t++) {
        pthread_create(&thread_array[t], NULL, countElms, &thread_ids[t]);
    }

    /* Join all the threads. Main will pause in this loop until all threads
    * have returned from the thread function. */
    for (t = 0; t < nthreads; t++) {
        pthread_join(thread_array[t], NULL);
    }

    free(thread_array);
    free(array);

    // sort my_array after all counting threads have finished
    writeArray();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
