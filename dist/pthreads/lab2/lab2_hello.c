#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

void *hello(void *arg)
{
    // static_assert(1+1==3);
    printf("hello\n");
    return NULL;
}

int main()
{
    pthread_t *p = malloc(1 * sizeof *p);
    pthread_create(p, NULL, hello, NULL);

    pthread_join(*p, NULL);
    return EXIT_SUCCESS;
}