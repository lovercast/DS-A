// test modding a floating pointer number
#include <stdio.h>
#include <stdlib.h>

int main()
{
    double a = (double)rand()/RAND_MAX; // generate a random float in [0, 1]
    double b = (double)rand()/RAND_MAX;

    printf("%g\n", a ); // can't take mod of double
    printf("%g\n", b );
    
}