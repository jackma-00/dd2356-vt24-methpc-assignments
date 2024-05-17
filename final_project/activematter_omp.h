#ifndef ACTIVEMATTER_H
#define ACTIVEMATTER_H

double square(double x);
void update_bird_positions(double *x, double *y, int N, int L);
void update_bird_velocities(double *vx, double *vy, double *theta, int N, float v0);
void move_birds(double *x, double *y, double *vx, double *vy, int N, int L, float dt);
double find_mean_angle_of_neighbors(double x_current, double y_current, double *theta, double *x, double *y, int N, int R);


#endif /* ACTIVEMATTER_H */