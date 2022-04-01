#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
/* MAX_THREADS - maksymalna liczba tworzonych wątków */
#define MAX_NUM_THREADS 4
/* MAX_NUM_PRIMES - przedział końcowy rozpatrywanych liczb */
#define MAX_NUM_PRIMES 1000

int primes[MAX_NUM_PRIMES];
int num_primes = 0;

/* isPrime(int num) - zwraca 1 jeśli liczba jest liczbą pierwszą, w przeciwnym wypadku zwraca 0 */
int isPrime(int num)
{
    int i;
    for (i = 0; i < num_primes; i++)
    {
        if (num % primes[i] == 0)
        {
            return 0;
        }
    }
    return 1;
}

/* ErastotenesSieve() - funkcja tworząca tablicę liczb pierwszych */
void erastotenesSieve()
{
    int i, j;
    for (i = 2; i <= MAX_NUM_PRIMES; i++)
    {
        if (isPrime(i))
        {
            primes[num_primes++] = i;
        }
    }
}

/* erastothenes_worker(void *arg) - funkcja wątku */
void *eratosthenesThread(void *arg)
{
    int i, j;
    int *thread_num = (int *)arg;
    /* start - indeks pierwszej liczby do sprawdzenia */
    int start = (*thread_num) * MAX_NUM_PRIMES / MAX_NUM_THREADS;
    /* end - indeks ostatniej liczby do sprawdzenia */
    int end = (*thread_num + 1) * MAX_NUM_PRIMES / MAX_NUM_THREADS;
    /* wyszukiwanie liczb pierwszych */
    for (i = start; i < end; i++)
    {
        /* sprawdzenie czy liczba jest liczbą pierwszą */
        for (j = 0; j < num_primes; j++)
        {
            if (i % primes[j] == 0)
            {
                break;
            }
        }
        /* jeśli liczba jest pierwsza */
        if (j == num_primes)
        {
            primes[num_primes++] = i;
        }
    }
    return NULL;
}

int main()
{
    /* inicjalizacja zmiennych */
    erastotenesSieve();
    pthread_t threads[MAX_NUM_THREADS];
    int i;
    /* tworzenie wątków */
    for (i = 0; i < MAX_NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, eratosthenesThread, &i);
    }
    /* czekanie na zakończenie wątków */
    for (i = 0; i < MAX_NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    /* wypisanie liczb pierwszych */
    for (i = 0; i < num_primes; i++)
    {
        printf("%d\n", primes[i]);
    }
    return 0;
}
