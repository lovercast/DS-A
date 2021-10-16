#include <stdio.h>
#include <omp.h>

inline void matrix_mul_ordinary(const int M, const int N, const int i_start, const int i_stop,  // alternative algorithm
            const double (*restrict X)[M][N],
            const double (*restrict Y)[N][M],
                  double (*restrict Z)[M][M]);

int main()
{

    #pragma omp parallel {
        #pragma omp for schedule(dynamic, 4)
        
    }

}

inline void matrix_mul_ordinary(const int M, const int N, const int i_start, const int i_stop,  // alternative algorithm
            const double (*restrict X)[M][N],
            const double (*restrict Y)[N][M],
                  double (*restrict Z)[M][M])
{
    int xrow, i, ycol;
    for (xrow = 0; xrow < M; ++xrow) {
        for (i = i_start; i < i_stop; ++i) {
            for (ycol = 0; ycol < M; ++ycol) {
                (*Z)[xrow][ycol] += (*X)[xrow][i] * (*Y)[i][ycol];
            }
        }
    }

}