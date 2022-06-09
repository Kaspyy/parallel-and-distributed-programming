#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi/mpi.h>

void main(int argc, char **argv)
{
    int rank, size, source, tag, i, root = 0;
    int max_chunks = 0;

    MPI_Status status;

    /* Inicjacja MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
    {
        printf("Podaj maksymalną liczbę wyrazów szeregu\n");
        scanf("%d", &max_chunks);
    }

    /* Wysłanie liczby wyrazów szeregu do wszystkich procesów */
    MPI_Bcast(&max_chunks, 1, MPI_INT, root, MPI_COMM_WORLD);

    /* Liczba wyrazów szeregu dla danego procesu
       ceil - zaokrąglenie w górę */
    int num = ceil(max_chunks / size);

    int start = rank * num;
    int end = start + num - 1;

    if (rank == (size - 1))
    {
        end = max_chunks;
    }

    double partial_sum = 0.0;

    double sum_plus = 0.0;
    double sum_minus = 0.0;

    /* Wyliczenie sumy szeregu */
    for (i = start; i <= end; i++)
    {
        int j = 1 + 4 * i;

        sum_plus += 1.0 / j;
        sum_minus += 1.0 / (j + 2.0);
    }
    partial_sum = sum_plus - sum_minus;

    double suma = 0.0;

    /* Odebranie sumy szeregu od wszystkich procesów */
    MPI_Reduce(&partial_sum, &suma, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);

    if (rank == root)
    {
        printf("PI = %1.6lf\n", 4 * (sum_plus - sum_minus));
    }

    MPI_Finalize();
}