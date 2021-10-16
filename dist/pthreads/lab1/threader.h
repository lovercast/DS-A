#ifndef THREADER_H
#define THREADER_H

#include <stdio.h>
#include <stdlib.h>

struct thread_args {
    int *array;
    long length;
    long num_threads;
    long id;
    long target;
};

#endif
