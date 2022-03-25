#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

/* Definiowanie własnego typu bool (niewystępującego domyślnie w języku C), dla czytelności kodu */
typedef enum
{
    false,
    true
} bool;

/* Definiowanie parametrów symulacji */
#define MUGS 2
#define CLIENTS 4
#define MUGS_DESIRED 2

/* Dopisywanie do zmiennej freeMugs liczby wolnych kufli */
int freeMugs = MUGS;

/* Inicjalizacja mutexów do realizacji sekcji krytycznych
 (sprawdzania wolnych kufli i zamawiania piwa) */
pthread_mutex_t checkingForMugs = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t orderingBeer = PTHREAD_MUTEX_INITIALIZER;

/* Wątek reprezentujący klienta */
void *clientThread(void *clientArrId)
{
    int clientId = *((int *)clientArrId);
    /* Aktualnie pity kufel piwa */
    int currentMug = 1;
    /* Zmienna definiująca dostępność kufla */
    bool isFreeMug = false;

    /* Pętla wykonująca się dopóki dany klient wypije żądaną liczbę kufli piwa */
    while (currentMug <= MUGS_DESIRED)
    {
        pthread_mutex_lock(&checkingForMugs);
        if (freeMugs > 0)
        {
            freeMugs--;
            isFreeMug = 1;
        }
        pthread_mutex_unlock(&checkingForMugs);

        /* Sekcja zamawiania piwa, strzeżona przez warunek sprawdzający czy jest wolny kufel */
        if (isFreeMug == true)
        {
            pthread_mutex_lock(&orderingBeer);
            printf("Klient nr %d zamawia piwo nr %d\n", clientId, currentMug);
            sleep(2);
            pthread_mutex_unlock(&orderingBeer);

            /* Symulacja czasu potrzebnego na wypicie piwa */
            int drinkingTime = rand() % 5;
            sleep(drinkingTime);

            printf("Klient nr %d oddaje kufel nr %d po czasie %d sekund\n", clientId, currentMug, drinkingTime);
            freeMugs++;
            currentMug++;
        }
        isFreeMug = false;
    }
}

void main()
{
    int i = 0;
    pthread_t *client;
    int *clientId;

    /* Alokacja pamięci dla wątków */
    client = (pthread_t *)malloc(CLIENTS * sizeof(pthread_t));
    clientId = (int *)malloc(CLIENTS * sizeof(int));

    for (i = 0; i < CLIENTS; i++)
        clientId[i] = i;

    printf("BAR OTWARTY!\n");

    /* Tworzenie wątków */
    for (i = 0; i < CLIENTS; i++)
        pthread_create(&client[i], NULL, clientThread, &clientId[i]);

    /* Dołączanie wątków */
    for (i = 0; i < CLIENTS; i++)
        pthread_join(client[i], NULL);

    printf("ZAMKNIETE!\n");

    pthread_exit(NULL);
}