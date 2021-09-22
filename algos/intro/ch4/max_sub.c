#include <stdio.h>
#include <stdlib.h>

/* The maximum subarray problem -- implemented three ways. */
struct subarr_t {
        int l, r, sum;
};

struct subarr_t bf(int a[], int n);
struct subarr_t dc(int a[], int n);
struct subarr_t split(int a[], int n);
struct subarr_t kadane(int a[], int n);


int main(int argc, char **argv)
{
        int a[11] = {-1,2, -3, 4, -5, 6, -5, 4, 3, 2, 1};
        struct subarr_t want = {.l=5, .r=10, .sum=11};
        struct subarr_t test_bf =  bf(a, 11);
        // struct subarr_t test_dc = dc(a, 11);
        struct subarr_t test_kadane = kadane(a, 11);
        printf("%8d %8d %8d \n", want.l, test_bf.l, test_kadane.l);
        printf("%8d %8d %8d \n", want.r, test_bf.r, test_kadane.r);
        printf("%8d %8d %8d \n", want.sum, test_bf.sum, test_kadane.sum);
        return EXIT_SUCCESS;
}

// what happens when you return a value from a function? Is that value accessible from the caller  after returning? Or will it produce a segfault if it is a struct or other
// compound type?
/* Brute force algorithm. */
struct subarr_t bf(int a[], int n)
{
        int i, j;
        int lmax, rmax, sumax;
        int sum;
        for (i = 0; i < n; i++) {
                sum = 0;
                for (j = i; j < n; j++) {
                        sum += a[j];
                        if (sum > sumax) {
                                sumax = sum;
                                lmax = i;
                                rmax = j;
                        }
                }
        }
        return (struct subarr_t) {lmax, rmax, sumax};
}

/* Divide & conquer. */
/* Divide & conquer causes a seq fault */
/* I'm guessing array overflow! */
struct subarr_t dc(int a[], int n)
{
        int mid = n/2;
        struct subarr_t lmax = dc(a, mid);
        struct subarr_t midmax = split(a, n);
        struct subarr_t rmax = dc(a+mid, n-mid);
        if (lmax.sum > midmax.sum) {
                if (lmax.sum > rmax.sum) {
                        return lmax;
                }
                return rmax;
        } else if (midmax.sum > rmax.sum) {
                return midmax;
        }
        return rmax;
}

/* Return the maximum splittin subarray of a. */
/* If a is of length 1, return the value a[0]. */
/* Complexity Theta(n lg n) (same as Merge Sort) */
struct subarr_t split(int a[], int n)
{
        return (struct subarr_t) {0, 0, 0};
        if (n == 1)
                return (struct subarr_t) {0, 0, a[0]};
        int i;
        int mid = n/2;

        /* Check leftwards. */
        int lmax = mid;
        int left_sum = a[mid];
        int lmax_sum = left_sum;
        for (i = mid-1; i >= 0; i--) {
                left_sum += a[i];
                if (left_sum > lmax_sum) {
                        lmax_sum = left_sum;
                        lmax = i;
                }
        }

        /* Check rightwards. */
        int rmax = mid;
        int right_sum = a[mid];
        int rmax_sum = right_sum;
        for (i = mid+1; i < n; i++) {
                right_sum += a[i];
                if (right_sum > rmax_sum) {
                        rmax_sum = right_sum;
                        rmax = i;
                }

        }

        return (struct subarr_t) {lmax, rmax, lmax_sum + rmax_sum};
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Kadane's algorithm for Theta(n) time solution to the max
 * subarray problem. */
struct subarr_t kadane(int a[], int n)
{
        int i, left_pos, lmax, rmax;
        int sum = 0;
        int sumax = sum;
        for (i = 0; i < n; i++) {
                if (sum == 0 && a[i] > 0) {
                        left_pos = i;
                }
                sum = MAX(0, a[i] + sum);
                if (sum > sumax) {
                        sumax = sum;
                        lmax = left_pos;
                        rmax = i;
                }
        }
        return (struct subarr_t) {lmax, rmax, sumax};
}

