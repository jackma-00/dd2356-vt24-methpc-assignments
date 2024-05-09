
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

#define SEED 921

int main(int argc, char *argv[])
{
    int flip = 1 << 24; // Number of trials = 2^24
    int rank, num_ranks, provided;
    double local_count, total_count, pi;
    double start_time, stop_time, loc_elapsed_time, elapsed_time;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    srand(SEED * rank);

    flip = flip / num_ranks; // Each process takes only a part of the samples

    MPI_Barrier(MPI_COMM_WORLD);

    start_time = MPI_Wtime();
    // Calculate PI following a Monte Carlo method
    local_count = Toss(flip);
    stop_time = MPI_Wtime();
    loc_elapsed_time = stop_time - start_time;

    MPI_Reduce(&loc_elapsed_time, &elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_count, &total_count, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Estimate Pi
    if (rank == 0)
        pi = ((double)total_count / (double)(flip * num_ranks)) * 4.0;

    MPI_Finalize();

    printf("The result is %f\n", pi);
    printf("Execution Time: %f\n", elapsed_time);

    return 0;
}

/**
 * Toss function simulates tossing darts at a dartboard.
 * It generates random (x, y) points and checks if they fall within a unit circle.
 * The function counts the number of points that fall within the circle and returns the count.
 *
 * @param flip The number of times to toss the dart.
 * @return The count of points that fall within the unit circle.
 */
int Toss(int flip)
{
    double x, y, z, local_count = 0;
    for (int iter = 0; iter < flip; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x * x) + (y * y));

        // Check if point is in unit circle
        if (z <= 1.0)
        {
            local_count++;
        }
    }
    return local_count;
}
