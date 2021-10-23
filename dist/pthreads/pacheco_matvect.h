/* This file was automatically generated.  Do not edit! */
#include <assert.h>
#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

const int m = 1000, n = 1000;
int nthreads;
int A[m][n], B[m * n], x[n], y1[n], y2[n];

#undef INTERFACE
void print_x(int x[n]);
void assert_ones(int v[n]);
void *Pth_mat_vect_B(void *rank);
void *Pth_mat_vect_A(void *rank);
void init_x(int x[n]);
extern const int m;
extern int A[m][n], B[m * n], x[n], y1[n], y2[n];
void identify_1d(int A[m * n]);
void identify(int A[m][n]);
extern int nthreads;
