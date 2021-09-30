/* Indexing a billion member array in row major and column major order. What is the difference in time of execution? */
#include <stdlib.h>

#define ARRLEN 10000
int **rowmajor;

int main(int argc, char **argv)
{
    int i, j;
    /* Allocate memory for the array. */
    rowmajor = malloc(sizeof(int *) * ARRLEN);
    for (i = 0; i < ARRLEN; i++)
        rowmajor[i] = malloc(sizeof(int) * ARRLEN);

    /* Incrementing elements in row-major order. */
        for (i = 0; i < ARRLEN; i++) {
                for (j = 0; j < ARRLEN; j++) {
                        rowmajor[i][j]++;
                }
        }
        return 0;
}

