#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int message = 45;
    MPI_Status status;

    if (rank == 0) {
        // Отправляем число процессу 1
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        // Принимаем от процесса 0
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("receive message '%d'\n", message);
    }

    MPI_Finalize();
    return 0;
}