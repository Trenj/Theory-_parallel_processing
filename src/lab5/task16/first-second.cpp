#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int token = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ждём своей очереди от предыдущего процесса
    if (rank != 0) {
        MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Выводим свою строку
    if (rank == 0) {
        printf("%d processes.\n", size);
    }
    if (rank % 2 == 0) {
        printf("I am %d process: FIRST!\n", rank);
    } else {
        printf("I am %d process: SECOND!\n", rank);
    }

    // Передаём эстафету следующему
    if (rank != size - 1) {
        MPI_Send(&token, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}