#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "activematter_omp.h"

int main(void)
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

    // Initialize
    srand(17); // seed

    // bird positions
    update_bird_positions(x, y, N, L);

    // bird velocities
    update_bird_velocities(vx, vy, theta, N, v0);

    // Simulation Main  Loop
    for (int i = 0; i < Nt; i++)
    {
        // move
        move_birds(x, y, vx, vy, N, L, dt);

        // Initialize mean_theta
        for (int j = 0; j < N; j++)
        {
            mean_theta[j] = theta[j];
        }

        // find mean angle of neighbors within R
        for (int b = 0; b < N; b++)
        {

            mean_theta[b] = find_mean_angle_of_neighbors(x[b], y[b], theta, x, y, N, R);
        }

        for (int b = 0; b < N; b++)
        {
            // add random perturbations
            theta[b] = mean_theta[b] + eta * (rand() / (double)RAND_MAX - 0.5);

            // update velocity
            vx[b] = v0 * cos(theta[b]);
            vy[b] = v0 * sin(theta[b]);
        }
    }

    printf("Simulation done\n");

    // Write data to a text file
    FILE *fp;
    fp = fopen("data.txt", "w");
    if (fp == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        fprintf(fp, "%f %f %f %f\n", x[i], y[i], vx[i], vy[i]);
    }

    fclose(fp);

    return 0;
}
