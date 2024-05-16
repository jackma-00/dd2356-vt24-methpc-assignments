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
    double x[N], y[N];
    for (int i = 0; i < N; i++){
        x[i] = rand()/(double)RAND_MAX *L;
        y[i] = rand()/(double)RAND_MAX *L;
    }

    // bird velocities
    double vx[N], vy[N];
    double theta;
    for (int i = 0; i < N; i++){
        theta = 2 * M_PI * rand()/(double) RAND_MAX;
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
        }

        for (int j = 0; j < N; j++){

            //find mean angle of neighbors within R
            float mean_theta = theta;
            for (int k = 0; k < N; k++){
                
            }

            //update velocity
            float angle = atan2(vy[j], vx[j]);
            angle += (2 * M_PI * rand() / RAND_MAX - M_PI) * eta;
            vx[j] = v0 * cos(angle);
            vy[j] = v0 * sin(angle);
        
        }


    }







