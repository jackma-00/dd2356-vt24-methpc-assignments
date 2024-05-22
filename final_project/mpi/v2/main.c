#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#include "activematter_mpi.h"

int main(int argc, char *argv[])
{

    // Simulation parameters
    float v0 = 1.0;           // velocity
    float eta = 0.5;          // random fluctuation in angle (in radians)
    int L = 10;               // size of box
    int R = 1;                // interaction radius
    float dt = 0.2;           // time step
    int Nt = 200;             // number of time steps
    int N = 500;              // number of birds
    bool plotRealTime = true; // plot real time or not

    // Simulation variables
    double x[N], y[N];    // bird positions
    double theta[N];      // bird angles
    double vx[N], vy[N];  // bird velocities
    double mean_theta[N]; // mean angle of neighbors

    // MPI variables
    int rank, num_ranks, provided;
    int birds_per_proc = N / num_ranks;
    int start = rank * birds_per_proc;
    int end = (rank == num_ranks - 1) ? N : start + birds_per_proc;

    // Initialize MPI
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    // Seed the random number generator
    srand(17);

    // bird positions
    update_bird_positions(x, y, N, L);

    // bird velocities
    update_bird_velocities(vx, vy, theta, N, v0);

    // Simulation Main  Loop
    for (int i = 0; i < Nt; i++)
    {
        // move
        move_birds(x, y, vx, vy, N, L, dt);

        // find mean angle of neighbors within R
        for (int b = start; b < end; b++)
        {

            mean_theta[b] = find_mean_angle_of_neighbors(rank, num_ranks, x[b], y[b], theta, x, y, N, R);
        }

        // Synchronize the mean_theta values across all processes
        MPI_Allgather(MPI_IN_PLACE, birds_per_proc, MPI_DOUBLE, mean_theta, birds_per_proc, MPI_DOUBLE, MPI_COMM_WORLD);

        for (int b = start; b < end; b++)
        {
            // add random perturbations
            theta[b] = mean_theta[b] + eta * (rand() / (double)RAND_MAX - 0.5);

            // update velocity
            vx[b] = v0 * cos(theta[b]);
            vy[b] = v0 * sin(theta[b]);
        }
    }

    // Finalize MPI
    MPI_Finalize();

    printf("Simulation done\n");

    // Write data to a text file
    FILE *fp;
    fp = fopen("data.txt", "w");
    if (fp == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < N; i++)
    {
        fprintf(fp, "%f %f %f %f\n", x[i], y[i], vx[i], vy[i]);
    }

    fclose(fp);

    return 0;
}
