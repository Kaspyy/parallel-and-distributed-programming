#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int global_variable = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *arg)
{
    int i, tmp;
    for (i = 0; i < 100000; i++)
    {
        pthread_mutex_lock(&mutex);
        tmp = global_variable;
        tmp++;
        printf("%d\n", tmp);
        global_variable = tmp;
        pthread_mutex_unlock(&mutex);
    }
}

void *decrement(void *arg)
{
    int i, tmp;
    for (i = 0; i < 100000; i++)
    {
        pthread_mutex_lock(&mutex);
        tmp = global_variable;
        tmp--;
        printf("%d\n", tmp);
        global_variable = tmp;
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{

    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, increment, NULL);
    pthread_create(&tid2, NULL, decrement, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("global variable: %d\n", global_variable);
}