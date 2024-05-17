#include <math.h>
#include <stdlib.h>
#include <stdio.h>



/**
 * Calculates the square of a given number.
 *
 * @param x The number to be squared.
 * @return The square of the given number.
 */
double square(double x)
{
    return x * x;
}

/**
 * Updates the positions of birds in a given space.
 *
 * @param x     Pointer to the array storing the x-coordinates of the birds.
 * @param y     Pointer to the array storing the y-coordinates of the birds.
 * @param N     The number of birds.
 * @param L     The size of the space in which the birds move.
 */
void update_bird_positions(double *x, double *y, int N, int L)
{
    for (int i = 0; i < N; i++)
    {
        // random position
        x[i] = rand() / (double)RAND_MAX * L;
        y[i] = rand() / (double)RAND_MAX * L;
    }
}

/**
 * Updates the velocities of a group of birds.
 *
 * This function updates the velocities of a group of birds based on a given speed and a random angle.
 *
 * @param vx Pointer to the array of bird x-velocities.
 * @param vy Pointer to the array of bird y-velocities.
 * @param theta Pointer to the array of bird angles.
 * @param N The number of birds.
 * @param v0 The speed of the birds.
 */
void update_bird_velocities(double *vx, double *vy, double *theta, int N, float v0)
{
    for (int i = 0; i < N; i++)
    {
        // random angle
        theta[i] = 2 * M_PI * rand() / (double)RAND_MAX;

        // update velocity
        vx[i] = v0 * cos(theta[i]);
        vy[i] = v0 * sin(theta[i]);
    }
}

/**
 * Updates the positions of birds based on their velocities.
 *
 * @param x     Array of bird x-coordinates
 * @param y     Array of bird y-coordinates
 * @param vx    Array of bird x-velocities
 * @param vy    Array of bird y-velocities
 * @param N     Number of birds
 * @param L     Size of the simulation domain
 * @param dt    Time step size
 */
void move_birds(double *x, double *y, double *vx, double *vy, int N, int L, float dt)
{
    for (int i = 0; i < N; i++)
    {
        // update position
        x[i] += vx[i] * dt;
        y[i] += vy[i] * dt;

        // apply periodic BCs (Boundary Conditions)
        //printf("Before x[i] = %f     ", x[i]);
        //x[i] = fmod(x[i], (double) L);
        //y[i] = fmod(y[i], (double) L);

        if(x[i] < 0){
            x[i] = L + x[i];
        }else if(x[i] > L){
            x[i] = x[i] - L;
        }

        if(y[i] < 0){
            y[i] = L + y[i];
        }else if(y[i] > L){
            y[i] = y[i] - L;
        }



        //printf("x[i] = %f\n", x[i]);
    }
}

/**
 * Calculates the mean angle of the neighbors for a given bird.
 *
 * @param x_current_bird The x-coordinate of the current bird.
 * @param y_current_bird The y-coordinate of the current bird.
 * @param theta Array of angles for all birds.
 * @param x Array of x-coordinates for all birds.
 * @param y Array of y-coordinates for all birds.
 * @param N The total number of birds.
 * @param R The radius within which birds are considered neighbors.
 */
double find_mean_angle_of_neighbors(
    double x_current_bird,
    double y_current_bird,
    double *theta,
    double *x,
    double *y,
    int N,
    int R)
{
    double sx = 0, sy = 0; // sum of cos and sin of angles
 
    // Iterate over the neighbors
    for (int i = 0; i < N; i++)
    {
        if ((square(x[i] - x_current_bird) + square(y[i] - y_current_bird)) < square(R))
        {
            sx+=cos(theta[i]);
            sy+=sin(theta[i]);
        }
    }

    // return mean angle for the current bird
    return atan2(sy, sx);
}
