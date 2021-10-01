#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>


/* Thread function -- takes a struct  */

void test_sort(void);
int partition(int arr[], int left, int right );
void quicksort(int arr[], int left, int right);
void merge(int dest[], int a[], int b[], int alen, int blen);
void test_merge(void);
void generate_random(int arr[], int len);
void *sort(void *v);
void print_arr(int arr[], int len);

typedef struct thread_arg {
    int len;
    int *arr; // pointer to slice of array.
} thread_arg;

/* M a i n ! */
int main(int argc, char *argv[])
{
    int experiment[] = {16807,  282475249, 1622650073,  984943658, 1144108930, 470211272, 101027544, 1457850878, 1458777923, 2007237709 };
    int len = 10;

    print_arr(experiment, len);
    quicksort(experiment, 0, 10);
    print_arr(experiment, len);

    exit(0);

    /* Create an array of random integers of length 1000000.
     * Parsel the array among num_threads threads.
     * Have each thread individually sort their segment.
     * Then merge the sorted arrays. */
    pthread_t *thread_array;
    thread_arg *thread_args;

    long num_threads = strtol(argv[1], NULL, 10); // what's the api for strtol?
    long num_rands = strtol(argv[2], NULL, 10);

    thread_array = malloc(num_threads*sizeof(pthread_t));
    thread_args = malloc(num_threads*sizeof(thread_arg));

    int arr[num_rands];
    generate_random(arr, num_rands);

    /* Divide the array between num_threads threads */
    /* length of each segment is stride, except possibly last */
    /* length of last segment is num_rands - num_threads*stride */
    int stride = num_rands/num_threads;

    /* want to avoid -- threads doing synchronous read/write on the same array!! */
    /* Solution -- copy parts of list to separate buffers, then pass those buffers to the
     * threads to individually sort. */
     /* implement mutex lock on the array -- but how to do that? */
     /* I could implement a  linked list -- that has the advantage that
      * the memory is not adjacent and I don't have to specify in advance how
      * many arrays there are going to be. */
      /* Well, I'll try this and see if it is functional at all. */

    int i;
    for (i = 0; i < num_threads; i++) {
        // thread_arg *arg = malloc(sizeof(thread_arg));
        thread_arg arg;
        if (i < num_threads-1) {
            arg.len=stride;
            arg.arr=arr+i*stride;
        } else {
            arg.len=num_rands-num_threads*stride;
            arg.arr=arr+i*stride;
        }
        thread_args[i] = arg;
        pthread_create(&thread_array[i], NULL, sort, &thread_args[i]);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(thread_array[i], NULL);

    print_arr(arr, num_rands);

    return 0;

    /* Create an array the size of the whole array. 
     * Merge the arrays into the new array.
     */
    int new_arr[num_rands];
    int arrlen = 0;
    for (int i = 0; i < num_threads; i++) {
        int temp[thread_args[i].len + arrlen];
        merge(temp, new_arr, thread_args[i].arr, arrlen, thread_args[i].len);
        /* write temp to arrlen */
        arrlen += thread_args[i].len;
        for (int j = 0; j < arrlen; j++) 
            new_arr[i] = temp[i];
    }

    print_arr(new_arr, arrlen);

    free(thread_array);
    free(thread_args);
    return EXIT_SUCCESS;
}

/* Thread Function */
void *sort(void *v)
{
    thread_arg *arg = (thread_arg *) v;
    int len = arg->len;
    int *arr = arg->arr;
    quicksort(arr, 0, len);
    return NULL;
}

void generate_random(int arr[], int len)
{
    for (int i = 0; i < len; i++) {
        arr[i] = rand();
    }
}

static int tem;

void print_arr(int arr[], int len)
{
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void test_sort()
{
    int len = 10;
    int sample[len];
    for (int i = 0 ; i < len; i++) {
        sample[i] = len-i-1;
    }

    print_arr(sample, len);
    quicksort(sample, 0, len-1);
    print_arr(sample, len);
}

#define swap(arr, i, j) (tem = arr[i], arr[i] = arr[j], arr[j] = tem)

int partition(int arr[], int left, int right )
{
    int i, last, pivot;
    last = left-1;
    pivot = arr[right];

    for (i = left; i <= right; i++) { // <= right
        if (arr[i] <= pivot) {
            ++last;
            swap(arr, i, last);
        }
    }
    return last;
}

void quicksort(int arr[], int left, int right)
{
    if (left > right)
        return;
    int pidx = partition(arr, left, right);
    quicksort(arr, left, pidx-1);
    quicksort(arr, pidx+1, right);
}

void test_merge()
{
    int a[] = {2,4,6,8,10};
    int b[] = {1,3,5,7,9,11};
    int alen = 5;
    int blen = 6;

    printf("array a:\n");
    print_arr(a, alen);
    printf("array b:\n");
    print_arr(b, blen);

    int dest[alen+blen];
    merge(dest, a, b, alen, blen);

    printf("Result array: \n");
    print_arr(dest, alen+blen);
}

void merge(int dest[], int a[], int b[], int alen, int blen)
{
    int i = 0, j = 0;
    while (i < alen && j < blen) {
        if (a[i] < b[j]) {
            dest[i+j] = a[i];
            ++i;
        } else {
           dest[i+j] = b[j];
           ++j;
        }
    }
    while (i < alen) {
        dest[i + j] = a[i];
        ++i;
    }
    while (j < blen) {
        dest[i + j] = b[j];
        ++j;
    }
}
