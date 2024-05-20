#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

void update_bird_positions(double *x, double *y, int N, int L);
void update_bird_velocities(double *vx, double *vy, double *theta, int N, float v0);
void move_birds(double *x, double *y, double *vx, double *vy, int N, int L, float dt);
double find_mean_angle_of_neighbors(double x_current_bird, double y_current_bird, double *theta, double *x, double *y, int N, int R);

int main(int argc, char *argv[])
{
    // Simulation parameters
    float v0 = 1.0;
    float eta = 0.5;
    int L = 10;
    int R = 1;
    float dt = 0.2;
    int Nt = 200;
    int N = 500;
    bool plotRealTime = true;

    // MPI variables
    int rank, num_ranks, provided;

    // Initialize MPI
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    // Determine the number of birds per process
    int birds_per_proc = N / num_ranks;
    int start = rank * birds_per_proc;
    int end = (rank == num_ranks - 1) ? N : start + birds_per_proc;

    // Simulation variables
    double x[N], y[N], theta[N], vx[N], vy[N], mean_theta[N];

    // Seed the random number generator
    srand(17 * rank);

    // Initialize bird positions and velocities
    update_bird_positions(x, y, N, L);
    update_bird_velocities(vx, vy, theta, N, v0);

    // Simulation Main Loop
    for (int i = 0; i < Nt; i++)
    {
        // Move birds
        move_birds(x, y, vx, vy, N, L, dt);

        // Find mean angle of neighbors within R for local birds
        for (int b = start; b < end; b++)
        {
            mean_theta[b] = find_mean_angle_of_neighbors(x[b], y[b], theta, x, y, N, R);
        }

        // Synchronize the mean_theta values across all processes
        MPI_Allgather(MPI_IN_PLACE, birds_per_proc, MPI_DOUBLE, mean_theta, birds_per_proc, MPI_DOUBLE, MPI_COMM_WORLD);

        // Update velocities with new angles and random perturbations
        for (int b = start; b < end; b++)
        {
            theta[b] = mean_theta[b] + eta * (rand() / (double)RAND_MAX - 0.5);
            vx[b] = v0 * cos(theta[b]);
            vy[b] = v0 * sin(theta[b]);
        }
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
