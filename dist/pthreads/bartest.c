#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// pthread_barrier_t bar;

// fix tabbing in vscode
void* thread_fun(void* arg)
{
    long len = 100000;
    for (int i = 0; i < len; i++) {
        if (i == len / 2) { }
    }
}

int main()
{
}