#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "mpi.h"

#define LICZBA_PAKIETOW 1000

// skutruktura studenta
struct student
{
    char nazwisko[50];
    int numer_indeksu;
    double ocena;
};

int main(int argc, char **argv)
{
    int rank, ranksent, size, source, dest, tag, i, j;
    double start, end;
    MPI_Status status;                                             // status zwracany przez MPI_Recv
    struct student baza1[20], baza2[20];                           // tablica studentow
    MPI_Datatype rekord1_typ, rekord2_typ;                         // typy danych
    int rozmiar_blokow[3] = {50, 1, 1};                            // rozmiar blokow
    MPI_Datatype typy_danych[3] = {MPI_CHAR, MPI_INT, MPI_DOUBLE}; // typy danych
    MPI_Aint lb, zasieg, podstawa, offset[3], temp;                // zmienne pomocnicze

    MPI_Init(&argc, &argv);               // inicjalizacja MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // pobranie numeru procesu
    MPI_Comm_size(MPI_COMM_WORLD, &size); // pobranie liczby procesow

    MPI_Get_address(&baza1[0].nazwisko[0], &offset[0]);   // pobranie adresu pierwszego elementu tablicy
    MPI_Get_address(&baza1[0].numer_indeksu, &offset[1]); // pobranie adresu drugiego elementu tablicy
    MPI_Get_address(&baza1[0].ocena, &offset[2]);         // pobranie adresu trzeciego elementu tablicy

    podstawa = offset[0];
    for (i = 0; i < 3; i++) // obliczenie zasiegu
        offset[i] -= podstawa;

    MPI_Type_struct(3, rozmiar_blokow, offset, typy_danych, &rekord1_typ); // tworzenie typu danych
    MPI_Type_get_extent(rekord1_typ, &lb, &zasieg);                        // pobranie zasiegu typu danych

    MPI_Type_create_resized(rekord1_typ, lb, zasieg, &rekord2_typ); // zmiana rozmiaru typu danych

    MPI_Type_commit(&rekord2_typ); // zatwierdzenie typu danych

    if (rank != 0)
    {
        dest = 0; // wyslanie danych do procesu 0
        tag = 0;  // tag dla wysylania danych

        strcpy(baza1[2 * rank].nazwisko, "Kowalski"); // wpisanie danych do tablicy
        baza1[2 * rank].numer_indeksu = 99999;
        baza1[2 * rank].ocena = 5.0;

        strcpy(baza1[2 * rank + 1].nazwisko, "Nowak");
        baza1[2 * rank + 1].numer_indeksu = 43235;
        baza1[2 * rank + 1].ocena = 3.0;
    }
    if (rank == 0)
    {
        start = MPI_Wtime(); // poczatek czasu
    }
    MPI_Barrier(MPI_COMM_WORLD); // czekanie na wszystkie procesy
    if (rank != 0)
    {
        for (i = 0; i < LICZBA_PAKIETOW; i++)
        {
            MPI_Send(&baza1[2 * rank], 2, rekord2_typ, dest, tag, MPI_COMM_WORLD); // wyslanie danych
        }
    }
    else
    {
        for (i = 1; i < size; i++)
        {
            for (j = 0; j < LICZBA_PAKIETOW; j++)
            {
                MPI_Recv(&baza1[2 * i], 2, rekord2_typ, i,
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status); // odbieranie danych
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {
        end = MPI_Wtime(); // koniec czasu
        printf("Czas przesylania pakietow (typ zwykly): %lf \n", end - start);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    {
        int rozmiar, rozmiar_pakietu, rozmiar_komunikatu, pozycja;
        void *bufor;
        MPI_Pack_size(50, MPI_CHAR, MPI_COMM_WORLD, &rozmiar); // obliczenie rozmiaru pakietu
        rozmiar_pakietu = rozmiar;
        MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &rozmiar);
        rozmiar_pakietu += rozmiar;
        MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &rozmiar);
        rozmiar_pakietu += rozmiar;

        rozmiar_komunikatu = 2 * rozmiar_pakietu;
        bufor = (void *)malloc(rozmiar_komunikatu); // alokacja pamieci

        if (rank != 0)
        {
            dest = 0;
            tag = 0;
            pozycja = 0;
            for (i = 2 * rank; i <= 2 * rank + 1; i++)
            {
                MPI_Pack(&baza1[i].nazwisko[0], 50, MPI_CHAR,
                         bufor, rozmiar_komunikatu, &pozycja, MPI_COMM_WORLD); // pakowanie danych
                MPI_Pack(&baza1[i].numer_indeksu, 1, MPI_INT,
                         bufor, rozmiar_komunikatu, &pozycja, MPI_COMM_WORLD);
                MPI_Pack(&baza1[i].ocena, 1, MPI_DOUBLE,
                         bufor, rozmiar_komunikatu, &pozycja, MPI_COMM_WORLD);
            }
        }
        if (rank == 0)
        {
            start = MPI_Wtime();
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank != 0)
        {
            for (i = 0; i < LICZBA_PAKIETOW; i++)
            {
                MPI_Send(bufor, pozycja, MPI_PACKED, dest, tag, MPI_COMM_WORLD); // wyslanie danych
            }
        }
        else
        {
            for (j = 1; j < size; j++)
            {
                for (i = 0; i < LICZBA_PAKIETOW; i++)
                {
                    MPI_Recv(bufor, rozmiar_komunikatu, MPI_PACKED, j,
                             MPI_ANY_TAG, MPI_COMM_WORLD, &status); // odbieranie danych
                }
                pozycja = 0;
                for (i = 2 * j; i <= 2 * j + 1; i++)
                {
                    MPI_Unpack(bufor, rozmiar_komunikatu, &pozycja,
                               &baza2[i].nazwisko[0], 50, MPI_CHAR, MPI_COMM_WORLD); // rozpakowanie danych
                    MPI_Unpack(bufor, rozmiar_komunikatu, &pozycja,
                               &baza2[i].numer_indeksu, 1, MPI_INT, MPI_COMM_WORLD);
                    MPI_Unpack(bufor, rozmiar_komunikatu, &pozycja,
                               &baza2[i].ocena, 1, MPI_DOUBLE, MPI_COMM_WORLD);
                }
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {
        end = MPI_Wtime();
        printf("Czas przesylania pakietow (typ spakowany): %lf \n", end - start);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return (0);
}
