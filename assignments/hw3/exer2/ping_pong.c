#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_SIZE (1<<22)  // Maximum message size to test (4MB)
#define REPS 100           // Number of ping-pong repetitions

int main(int argc, char** argv) {
    int my_rank, size, provided;

    // Initialize MPI environment
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (my_rank == 0) {
            fprintf(stderr, "This program requires exactly 2 MPI processes\n");
        }
        MPI_Finalize();
        return -1;
    }

    double* buffer = (double*)malloc(MAX_SIZE);
    double start_time, end_time, elapsed_time;

    for (int size = sizeof(double); size <= MAX_SIZE; size *= 2) {
        MPI_Barrier(MPI_COMM_WORLD);

        start_time = MPI_Wtime();
        for (int i = 0; i < REPS; i++) {
            if (my_rank == 0) {
                MPI_Send(buffer, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(buffer, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Recv(buffer, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(buffer, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
            }
        }
        end_time = MPI_Wtime();
        elapsed_time = (end_time - start_time) / (2 * REPS); // Calculate average round-trip time
        if (my_rank == 0) {
            printf("Message Size: %d bytes, Average Ping-Pong Time: %f seconds\n", size, elapsed_time);
        }
    }

    free(buffer);
    MPI_Finalize();
    return 0;
}