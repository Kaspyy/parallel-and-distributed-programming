// Stworzenie programu mnożącego dwa wektory o zmiennych typu double (losowych) i długości 100.

// gcc

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int main()
{
    int i;
    double a[100000], b[100000], c[100000];
    for (i = 0; i < 100000; i++)
    {
        a[i] = (double)rand() / (double)RAND_MAX;
        b[i] = (double)rand() / (double)RAND_MAX;
    }

    omp_set_num_threads(4);

    double start = omp_get_wtime();

// zrównoleglenie
#pragma omp parallel for shared(a, b, c) private(i)
    for (i = 0; i < 100000; i++)
    {
        c[i] = a[i] * b[i];
    }

    double stop = omp_get_wtime();

    double time = stop - start;

    printf("Time (parallel for): %f\n", time);

    // set time
    start = omp_get_wtime();

// zwektoryzowanie
#pragma omp simd private(i)
    for (i = 0; i < 100000; i++)
        c[i] = a[i] * b[i];

    // get time
    stop = omp_get_wtime();
    time = stop - start;

    // print time
    printf("Time (simd): %f\n", time);

    return 0;
}