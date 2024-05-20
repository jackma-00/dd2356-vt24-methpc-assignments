#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#include "activematter_mpi.h"

double square(double x) {
    return x * x;
}

void update_bird_positions(double *x, double *y, int N, int L) {
    for (int i = 0; i < N; i++) {
        // random position
        x[i] = rand() / (double)RAND_MAX * L;
        y[i] = rand() / (double)RAND_MAX * L;
    }
}

void update_bird_velocities(double *vx, double *vy, double *theta, int N, float v0) {
    for (int i = 0; i < N; i++) {
        // random angle
        theta[i] = 2 * M_PI * rand() / (double)RAND_MAX;

        // update velocity
        vx[i] = v0 * cos(theta[i]);
        vy[i] = v0 * sin(theta[i]);
    }
}

void move_birds(double *x, double *y, double *vx, double *vy, int N, int L, float dt) {
    for (int i = 0; i < N; i++) {
        // update position
        x[i] += vx[i] * dt;
        y[i] += vy[i] * dt;

        // apply periodic BCs (Boundary Conditions)
        if (x[i] < 0) {
            x[i] = L + x[i];
        } else if (x[i] > L) {
            x[i] = x[i] - L;
        }

        if (y[i] < 0) {
            y[i] = L + y[i];
        } else if (y[i] > L) {
            y[i] = y[i] - L;
        }
    }
}

double find_mean_angle_of_neighbors(int rank, int num_ranks, double x_current_bird, double y_current_bird, double *theta, double *x, double *y, int N, int R) {
    double local_sx = 0, local_sy = 0; // Local sum of cos and sin of angles
    double total_sx = 0, total_sy = 0; // Total sum of cos and sin of angles

    int chunk_size = N / num_ranks;
    int start = rank * chunk_size;
    int end = (rank + 1) * chunk_size;

    // Synchronization point
    /*
        let's wait for all processes to get ready.
    */
    MPI_Barrier(MPI_COMM_WORLD);

    // Iterate over the neighbors
    for (int iter = start; iter < end; iter++) {
        if ((square(x[iter] - x_current_bird) + square(y[iter] - y_current_bird)) < square(R)) {
            local_sx += cos(theta[iter]);
            local_sy += sin(theta[iter]);
        }
    }

    // let's gather all data back to all processes. 
    // Reduce the local sums to the total sum
    MPI_Allreduce(&local_sx, &total_sx, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(&local_sy, &total_sy, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // Calculate the mean angle
    return atan2(total_sy, total_sx);
}
