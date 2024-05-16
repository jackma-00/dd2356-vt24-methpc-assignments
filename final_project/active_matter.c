#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

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

    // Initialize
    srand(17); // seed

    // bird positions
    double x[N], y[N];
    update_bird_positions(&x, &y, N, L);

    // bird velocities
    double vx[N], vy[N];
    double theta[N];
    update_bird_velocities(&vx, &vy, &theta, N, v0);

    // Simulation Main  Loop
    for (int i = 0; i < Nt; i++)
    {
        // move
        move_birds(&x, &y, &vx, &vy, N, L, dt);
        
        double mean_theta[N];
        
        for (int i = 0; i < N; i++)
        {
            mean_theta[i] = theta[i];
        }

        for (int j = 0; j < N; j++)
        {

            // find mean angle of neighbors within R
            double sx[N], sy[N];

            find_mean_angle_of_neighbors(x[j],y[j],&mean_theta, &theta, &x, &y, N, R, &sx, &sy);

            // add random perturbations
            theta[j] = mean_theta[j] + eta * (rand() / (double)RAND_MAX - 0.5);

            // update velocity
            
            vx[j] = v0 * cos(theta[j]);
            vy[j] = v0 * sin(theta[j]);
        }
    }

    printf("Simulation done\n");

    return 0;
}

double square(double x)
{
    return x * x;
}

void update_bird_positions(double *x, double *y, int N, int L){
    for (int i = 0; i < N; i++){
        x[i] = rand()/(double)RAND_MAX *L;
        y[i] = rand()/(double)RAND_MAX *L;
    }

}

void update_bird_velocities(double *vx, double *vy, double *theta, int N, float v0){
    for (int i = 0; i < N; i++){
        theta[i] = 2 * M_PI * rand()/(double) RAND_MAX;
        vx[i] = v0 * cos(theta[i]);
        vy[i] = v0 * sin(theta[i]);
    }
}

void move_birds(double *x, double *y, double *vx, double *vy, int N, int L, float dt){
    for (int i = 0; i < N; i++){
        x[i] += vx[i] * dt;
        y[i] += vy[i] * dt;

        // apply periodic BCs (Boundary Conditions)
        x[i] = fmod(x[i], L);
        y[i] = fmod(y[i], L);
    }
}

void find_mean_angle_of_neighbors(double x_val, double y_val, double *mean_theta, double *theta, double *x, double *y, int N, int R, double *sx, double *sy){
    
    for (int k = 0; k < N; k++)
    {
        if ((square(x_val - x[k]) + square(y_val - y[k])) < square(R))
        {
            sx[k] = sx[k] + cos(theta[k]);
            sy[k] = sy[k] + sin(theta[k]);
            mean_theta[k] = mean_theta[k] + atan2(sy[k], sx[k]);
        }
    }
}
