
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]){

    int rank, size, i, provided;
    
    // number of cells (global)
    int nxc = 128; // make sure nxc is divisible by size
    double L = 2*3.1415; // Length of the domain
    

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // number of nodes (local to the process): 0 and nxn_loc-1 are ghost cells 
    int nxn_loc = nxc/size + 3; // number of nodes is number cells + 1; we add also 2 ghost cells
    double L_loc = L/((double) size);
    double dx = L / ((double) nxc);
    
    // define out function
    double *f = calloc(nxn_loc, sizeof(double)); // allocate and fill with z
    double *dfdx = calloc(nxn_loc, sizeof(double)); // allocate and fill with z

    for (i=1; i<(nxn_loc-1); i++)
      f[i] = sin(L_loc*rank + (i-1) * dx);
    
    // need to communicate and fill ghost cells f[0] and f[nxn_loc-1]
    // communicate ghost cells
	int nxt,prev;
	nxt = rank +1;
	prev = rank -1;
	//first node
	if (rank == 0){
		prev = size-1;
	//last node
	}else if (rank == size -1){
		nxt = 0;
	}

	//comunicate
	//MPI_Ssend
	
	//Even nodes first send and then receive
	if (rank%2==0){
		MPI_Ssend(&f[2], 1, MPI_DOUBLE, prev, 0, MPI_COMM_WORLD);
		MPI_Ssend(&f[nxn_loc-3], 1, MPI_DOUBLE, nxt, 1, MPI_COMM_WORLD);
		
    		MPI_Recv(&f[nxn_loc-1], 1, MPI_DOUBLE, nxt, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    		MPI_Recv(&f[0], 1, MPI_DOUBLE, prev, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
	}else{
	
    		MPI_Recv(&f[nxn_loc-1], 1, MPI_DOUBLE, nxt, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    		MPI_Recv(&f[0], 1, MPI_DOUBLE, prev, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
		
		MPI_Ssend(&f[2], 1, MPI_DOUBLE, prev, 0, MPI_COMM_WORLD);
		MPI_Ssend(&f[nxn_loc-3], 1, MPI_DOUBLE, nxt, 1, MPI_COMM_WORLD);

	}


    for (i=1; i<(nxn_loc-1); i++)
      dfdx[i] = (f[i+1] - f[i-1])/(2*dx);

    
    if (rank==0){ // print only rank 0 for convenience
        printf("My rank %d of %d\n", rank, size );
        printf("Here are my values for f including ghost cells\n");
        for (i=0; i<nxn_loc; i++)
	       printf("%f\n", f[i]);
        printf("\n");
    }   



    //Test Ghostcells if they are correct

    if (fabs(f[0] - sin(L_loc*rank - dx)) > 0.01 || fabs(f[nxn_loc-1] - sin(L_loc*(rank+1) + dx)) > 0.01 ) {
        printf("Wrong ghost cell, First ghost is f[0] = %f,  Last ghost is f[last] = %f with a rank = %d \n correct f[0] = %f, f[last] = %f \n", f[0], f[nxn_loc-1], rank, sin(L_loc*rank - dx), sin(L_loc*(rank+1) + dx));
    }
    else printf("Correct ghost cell for rank %d \n", rank);

    //Testing derivatives on the edges

    if (rank == size-1) {
        if (fabs(cos(L_loc*rank + (nxn_loc-2 -1) * dx) - dfdx[nxn_loc-2]) > 0.01) {
            printf("Right derivative wrong, should be %f is %f \n", cos(L_loc*rank + (nxn_loc - 2 - 1) * dx), dfdx[nxn_loc-2]);
        }
        else printf("Right derivative correct\n");
    }
    if (rank==0) {
        if (fabs(cos(0) - dfdx[1]) > 0.01) {
            printf("Left derivative wrong, should be %f is %f \n", cos(0), dfdx[1]);
        }
        else printf("Left derivative correct\n");
    }

    MPI_Finalize();
}





