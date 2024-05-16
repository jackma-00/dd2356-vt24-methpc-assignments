#include <math.h>
#include <stdlib.h>

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
        x[i] = fmod(x[i], L);
        y[i] = fmod(y[i], L);
    }
}

/**
 * Calculates the mean angle of the neighbors for a given bird.
 *
 * @param x_current_bird The x-coordinate of the current bird.
 * @param y_current_bird The y-coordinate of the current bird.
 * @param mean_theta Pointer to the variable where the mean angle will be stored.
 * @param theta Array of angles for all birds.
 * @param x Array of x-coordinates for all birds.
 * @param y Array of y-coordinates for all birds.
 * @param N The total number of birds.
 * @param R The radius within which birds are considered neighbors.
 * @param sx_values Array to store the cos values of the angles for the neighbors.
 * @param sy_values Array to store the sin values of the angles for the neighbors.
 * @param b Index of the current bird.
 */
void find_mean_angle_of_neighbors(
    double x_current_bird,
    double y_current_bird,
    double *mean_theta,
    double *theta,
    double *x,
    double *y,
    int N,
    int R,
    double *sx_values,
    double *sy_values,
    int b)
{
    double sx = 0, sy = 0; // sum of cos and sin of angles

    // Iterate over the neighbors
    for (int i = 0; i < N; i++)
    {
        if ((square(x[i] - x_current_bird) + square(y[i] - y_current_bird)) < square(R))
        {
            sx_values[i] = cos(theta[i]);
            sy_values[i] = sin(theta[i]);
        }
    }

    // sum the cos and sin values
    for (int i = 0; i < N; i++)
    {
        sx += sx_values[i];
        sy += sy_values[i];
    }

    // calculate mean angle for the current bird
    mean_theta[b] = atan2(sy, sx);
}