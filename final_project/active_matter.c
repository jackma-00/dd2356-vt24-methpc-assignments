#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>


int main(int){



    //Simulation parameters
    float v0 = 1.0; //velocity
    float eta = 0.5; //random fluctuation in angle (in radians)
    int L = 10; //size of box
    int R = 1; //interaction radius
    float dt = 0.2; //time step
    int Nt = 200; //number of time steps
    int N = 500; //number of birds
    bool plotRealTime = true; //plot real time or not

    //Initialize
    srand(17); //seed

    // bird positions
    float x[N], y[N];
    for (int i = 0; i < N; i++){
        x[i] = rand() % L;
        y[i] = rand() % L;
    }

    // bird velocities
    float vx[N], vy[N];
    float theta
    for (int i = 0; i < N; i++){
        theta = 2 * M_PI * rand() / RAND_MAX;
        vx[i] = v0 * cos(theta);
        vy[i] = v0 * sin(theta);
    }

    // Simulation Main  Loop
    for (int i = 0; i < Nt; i++) {
    
        //move
        for (int j = 0; j < N; j++){
            //update position
            x[j] += vx[j] * dt;
            y[j] += vy[j] * dt;

            //apply periodic BCs (Boundary Conditions)
            x[j] = fmod(x[j], L);
            y[j] = fmod(y[j], L);

            //find mean angle of neighbors within R
            float mean_theta = theta;
            for (int k = 0; k < N; k++){
                if (k != j){
                    float dx = x[k] - x[j];
                    float dy = y[k] - y[j];
                    if (dx > L/2) dx -= L;
                    if (dy > L/2) dy -= L;
                    if (dx < -L/2) dx += L;
                    if (dy < -L/2) dy += L;
                    if (sqrt(dx*dx + dy*dy) < R){
                        mean_theta += atan2(dy, dx);
                    }
                }
            }

            //update velocity
            float angle = atan2(vy[j], vx[j]);
            angle += (2 * M_PI * rand() / RAND_MAX - M_PI) * eta;
            vx[j] = v0 * cos(angle);
            vy[j] = v0 * sin(angle);
        }


    }




