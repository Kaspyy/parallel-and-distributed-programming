#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long long
fibonacci(long n)
{
    if (n < 2)
        return n;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
}

long long fibonacciOmp(long n)
{
    long long i, j;
#pragma omp parallel
    {
#pragma omp single
        {
#pragma omp task shared(i)
            if (n < 38)
                i = fibonacci(n - 1);
            else
                i = fibonacciOmp(n - 1);

#pragma omp task shared(j)
            if (n < 38)
                j = fibonacci(n - 2);
            else
                j = fibonacciOmp(n - 2);
#pragma omp taskwait
        }
    }
    return i + j;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    long n = atol(argv[1]);

    double start = omp_get_wtime();

    long long result = fibonacciOmp(n);

    double stop = omp_get_wtime();

    // print result
    printf("Fibonacci(%ld) = %lld\n", n, result);

    // print time
    printf("Time: %f\n", stop - start);

    return 0;
}