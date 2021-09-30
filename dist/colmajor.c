/* Indexing a billion member array in row major and column major order. What is the difference in time of execution? */
#include <stdlib.h>

#define ARRLEN 100000
int **colmajor;

int main(int argc, char **argv)
{
    int i, j;
    /* Allocate memory for the array. */
    colmajor = malloc(sizeof(int *) * ARRLEN);
    for (i = 0; i < ARRLEN; i++)
        colmajor[i] = malloc(sizeof(int) * ARRLEN);

        for (j = 0; j < ARRLEN; j++) {
                for (i = 0; i < ARRLEN; i++) {
                        colmajor[i][j]++;
                }
        }
        return 0;
}

