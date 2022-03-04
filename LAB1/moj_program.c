#include <stdio.h>
#include "pomiar_czasu.h"

void main()
{
    int i, suma = 0;

    inicjuj_czas();
    for (i = 0; i < 50000; i++) // pętla z operacją arytmetyczną
        suma += i;
    printf("Pomiar czasu wykonania petli z operacja arytmetyczna:\n");
    drukuj_czas();

    inicjuj_czas();

    for (i = 0; i < 50000; i++) // pętla z operacją we/wy
        printf("@");
    printf("\n");
    printf("Pomiar czasu wykonania petli z operacja we/wy:\n");
    drukuj_czas();
}