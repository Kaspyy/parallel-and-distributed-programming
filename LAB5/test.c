#include <stdio.h>
#include <omp.h>

int main()
{
    int i, a;
    a = 7;

    for (i = 0; i < 10; i++)
    {
        printf("Thread %d a=%d\n", omp_get_thread_num(), a);
        a++;
    }
    return 0;
}
