#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main()
{
    // multiply two matrices
    int n = 500;
    int m = 1000;
    int p = 1000;
    int i, j, k;
    double **A = (double **)malloc(n * sizeof(double *));
    double **B = (double **)malloc(m * sizeof(double *));
    double **C = (double **)malloc(p * sizeof(double *));
    for (i = 0; i < n; i++)
    {
        A[i] = (double *)malloc(m * sizeof(double));
    }
    for (i = 0; i < m; i++)
    {
        B[i] = (double *)malloc(p * sizeof(double));
    }
    for (i = 0; i < p; i++)
    {
        C[i] = (double *)malloc(p * sizeof(double));
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            A[i][j] = (double)rand() / RAND_MAX;
        }
    }
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < p; j++)
        {
            B[i][j] = (double)rand() / RAND_MAX;
        }
    }
    for (i = 0; i < p; i++)
    {
        for (j = 0; j < p; j++)
        {
            C[i][j] = 0;
        }
    }

    omp_set_num_threads(4);

    // start timer
    double start = omp_get_wtime();

    // #pragma omp parallel for schedule(dynamic) shared(A, B, C) private(i, j, k) // pętla zewnętrzna, dynamiczna strategia przydziału

    // #pragma omp parallel for schedule(static) shared(A, B, C) private(i, j, k) // pętla zewnętrzna, statyczna strategia przydziału

    // #pragma omp parallel for collapse(3) schedule(dynamic) shared(A, B, C) private(i, j, k) // pętla zewnętrzna i wewnętrzna dynamiczna strategia przydziału
#pragma omp parallel for collapse(3) schedule(static) shared(A, B, C) private(i, j, k) // pętla zewnętrzna i wewnętrzna statyczna strategia przydziału
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < p; j++)
        {
            // #pragma omp parallel for schedule(dynamic) // pętla wewnętrzna, dynamiczna strategia przydziału
            // #pragma omp parallel for schedule(static) // pętla wewnętrzna, statyczna strategia przydziału
            for (k = 0; k < m; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // stop timer
    double end = omp_get_wtime();

    // print time
    printf("Czas: %f\n", end - start);

    // free memory
    for (i = 0; i < n; i++)
    {
        free(A[i]);
    }
    free(A);
    for (i = 0; i < m; i++)
    {
        free(B[i]);
    }
    free(B);
    for (i = 0; i < p; i++)
    {
        free(C[i]);
    }
    free(C);

    return 0;
}