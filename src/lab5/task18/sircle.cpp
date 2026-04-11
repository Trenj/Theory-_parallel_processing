#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int message;
    MPI_Status status;

    int prev = (rank - 1 + size) % size;  // предыдущий процесс
    int next = (rank + 1) % size;          // следующий процесс

    if (rank == 0) {
        // Процесс 0 сначала отправляет, потом принимает
        message = 0;

        MPI_Send(&message, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&message, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
    } else {
        // Остальные сначала принимают, потом 
        
        MPI_Recv(&message, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
        int send_val = message + 1;
        MPI_Send(&send_val, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    printf("[%d] : receive message '%d'\n", rank, message);

    MPI_Finalize();
    return 0;
}