#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void ring(int rank, int size, MPI_Comm comm); // funkcja wysylajaca dane do sasiadow

int main(int argc, char **argv)
{
    int rank, size;                       // rank - numer procesu, size - liczba procesów
    MPI_Init(&argc, &argv);               // inicjalizacja MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // pobranie numeru procesu
    MPI_Comm_size(MPI_COMM_WORLD, &size); // pobranie liczby procesów
    ring(rank, size, MPI_COMM_WORLD);     // wywołanie funkcji ring
    MPI_Finalize();                       // zakończenie MPI
    return 0;
}

void ring(int rank, int size, MPI_Comm comm)
{
    int data;                   // dane do wysłania
    MPI_Status status;          // status wysłania
    MPI_Comm_rank(comm, &rank); // pobranie numeru procesu
    MPI_Comm_size(comm, &size); // pobranie liczby procesów
    if (rank == 0)
    {
        printf("Enter a positive number: ");
        scanf("%d", &data); // wczytanie liczby
        while (data > 0)
        {
            MPI_Send(&data, 1, MPI_INT, (rank + 1) % size, 0, comm);                    // wysłanie danych do następnego procesu
            MPI_Recv(&data, 1, MPI_INT, (rank + 1) % size, MPI_ANY_TAG, comm, &status); // odbior danych od następnego procesu
            printf("Process %d received %d from %d\n", rank, data, status.MPI_SOURCE);  // wypisanie informacji o odebranych danych
            printf("Enter a positive number: ");                                        // ponowne wczytanie liczby
            scanf("%d", &data);                                                         // wczytanie liczby
        }
    }
    else
    {
        MPI_Recv(&data, 1, MPI_INT, (rank - 1) % size, MPI_ANY_TAG, comm, &status); // odbior danych od poprzedniego procesu
        printf("Process %d received %d from %d\n", rank, data, status.MPI_SOURCE);  // wypisanie informacji o odebranych danych
        MPI_Send(&data, 1, MPI_INT, (rank + 1) % size, 0, comm);                    // wysłanie danych do następnego procesu
    }
}