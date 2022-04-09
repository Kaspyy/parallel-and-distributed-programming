#include <stdio.h>
#include <omp.h>
#define ITERATIONS 10000

int main()
{
    int i, a;
    a = 7;

    printf("Private\n");
#pragma omp parallel for private(a) num_threads(7)
    for (i = 0; i < 10; i++)
    {
        printf("Thread %d: a = %d\n", omp_get_thread_num(), a);
        a++;
    }
    printf("Zmienna a = %d\n", a);

    a = 7;

    printf("\nFirstprivate\n");
#pragma omp parallel for firstprivate(a) num_threads(7)
    for (i = 0; i < 10; i++)
    {
        printf("Thread %d a = %d\n", omp_get_thread_num(), a);
        a++;
    }
    printf("Zmienna a = %d\n", a);

    a = 7;
    printf("\nShared\n");
#pragma omp parallel for shared(a) num_threads(7)
    for (i = 0; i < 10; i++)
    {
        printf("Thread %d a = %d\n", omp_get_thread_num(), a);
        a++;
    }
    printf("Zmienna a = %d\n", a);

    // Napisanie procedury, w której znajdą się cztery pętle o 15 iteracjach testujące różne strategie przydziału iteracji czterem wątkom :
    // • static, rozmiar porcji = 3,
    // • static, rozmiar porcji domyślny,
    // • dynamic rozmiar porcji = 3,
    // • dynamic, rozmiar porcji domyślny.

    double start,
        end;
    printf("\nStatic, rozmiar porcji=3\n");
    start = omp_get_wtime();
#pragma omp parallel for schedule(static, 3) num_threads(7)
    for (i = 0; i < ITERATIONS; i++)
    {
        // printf("Thread %d: %d\n", omp_get_thread_num(), i);
        a++;
    }
    end = omp_get_wtime();
    printf("Czas wykonania: %f\n", end - start);

    a = 7;
    printf("\nStatic, rozmiar porcji domyślny\n");
    start = omp_get_wtime();
#pragma omp parallel for schedule(static) num_threads(7)

    for (i = 0; i < ITERATIONS; i++)
    {
        // printf("Thread %d i = %d\n", omp_get_thread_num(), i);
        a++;
    }
    end = omp_get_wtime();
    printf("Czas wykonania: %f\n", end - start);

    a = 7;
    printf("\nDynamic, rozmiar porcji=3\n");
    start = omp_get_wtime();
#pragma omp parallel for schedule(dynamic, 3) num_threads(7)
    for (i = 0; i < ITERATIONS; i++)
    {
        // printf("Thread %d i = %d\n", omp_get_thread_num(), i);
        a++;
    }
    end = omp_get_wtime();
    printf("Czas wykonania: %f\n", end - start);

    a = 7;
    printf("\nDynamic, rozmiar porcji domyślny\n");
    start = omp_get_wtime();
#pragma omp parallel for schedule(dynamic) num_threads(7)
    for (i = 0; i < ITERATIONS; i++)
    {
        // printf("Thread %d i = %d\n", omp_get_thread_num(), i);
        a++;
    }
    end = omp_get_wtime();
    printf("Czas wykonania: %f\n", end - start);

    int sum = 0;
    int n = 500;
    printf("\nSuma 500 kwadratów liczby (reduction)\n");
    start = omp_get_wtime();
#pragma omp parallel for reduction(+ \
                                   : sum) num_threads(7)
    for (i = 0; i < n; i++)
    {
        sum += i * i;
    }
    end = omp_get_wtime();
    printf("Czas wykonania: %f\n", end - start);

    sum = 0;
    printf("\nSuma 500 kwadratów liczby (critical)\n");
    start = omp_get_wtime();
#pragma omp parallel for num_threads(7)
    for (i = 0; i < n; i++)
    {
#pragma omp critical
        {
            sum += i * i;
        }
    }
    end = omp_get_wtime();
    printf("Czas wykonania: %f\n", end - start);

    sum = 0;
    printf("\nSuma 500 kwadratów liczby (lock)\n");
    start = omp_get_wtime();
#pragma omp parallel for num_threads(7)
    for (i = 0; i < n; i++)
    {
        omp_lock_t lock;
        omp_init_lock(&lock);
        omp_set_lock(&lock);
        sum += i * i;
        omp_unset_lock(&lock);
        omp_destroy_lock(&lock);
    }
    end = omp_get_wtime();
    printf("Czas wykonania: %f\n", end - start);

    return 0;
}
