#ifndef ACTIVEMATTER_MPI_H
#define ACTIVEMATTER_MPI_H

double square(double x);
void update_bird_positions(double *x, double *y, int N, int L);
void update_bird_velocities(double *vx, double *vy, double *theta, int N, float v0);
void move_birds(double *x, double *y, double *vx, double *vy, int N, int L, float dt);
double find_mean_angle_of_neighbors(int rank, int num_ranks, double x_current_bird, double y_current_bird, double *theta, double *x, double *y, int N, int R);

#endif // ACTIVEMATTER_MPI_H
