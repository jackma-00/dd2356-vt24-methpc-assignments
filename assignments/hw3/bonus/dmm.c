#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include "fox.h"

int main(int argc, char *argv[])
{

    int i, j, dim;
    double **localA;
    double **localB;
    double **localC;
    int rank, num_ranks, provided;
    double start_time, stop_time, loc_elapsed_time, elapsed_time;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    GridInfo grid;
    /*initialize Grid */

    SetupGrid(&grid);
    /* Initialize matrix A & B */
    initialiseAB();
    /* calculate local matrix dimension */
    dim = N / grid.q;
    /* allocate space for the three matrices */

    localA = (double **)malloc(dim * sizeof(double *));

    localB = (double **)malloc(dim * sizeof(double *));

    localC = (double **)malloc(dim * sizeof(double *));

    for (i = 0; i < dim; i++)
    {
        *(localA + i) = (double *)malloc(dim * sizeof(double));
        *(localB + i) = (double *)malloc(dim * sizeof(double));
        *(localC + i) = (double *)malloc(dim * sizeof(double));
    }

    /* Compute local matrices - Ideally the master should do this & pass it onto all the slaves */
    /* At the same time initialize localC to all zeros */

    int base_row = grid.my_row * dim;
    int base_col = grid.my_col * dim;

    for (i = base_row; i < base_row + dim; i++)
    {
        for (j = base_col; j < base_col + dim; j++)
        {
            localA[i - (base_row)][j - (base_col)] = matrixA[i][j];
            localB[i - (base_row)][j - (base_col)] = matrixB[i][j];
            localC[i - (base_row)][j - (base_col)] = 0;
        }
    }

    start_time = MPI_Wtime();
    Fox(N, &grid, localA, localB, localC);
    stop_time = MPI_Wtime();
    loc_elapsed_time = stop_time - start_time;

    MPI_Reduce(&loc_elapsed_time, &elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        /* print results */
        printf("rank=%d, row=%d col=%d\n", grid.my_rank, grid.my_row, grid.my_col);
        for (i = 0; i < dim; i++)
        {
            for (j = 0; j < dim; j++)
            {
                // printf("localC[%d][%d]=%d ", i,j,localC[i][j]);
                printf("%f ", localC[i][j]);
            }
            printf("\n");
        }
        printf("Execution Time: %f\n", elapsed_time);
        printf("Number of processes: %d\n", num_ranks);
    }

    MPI_Finalize();
    exit(0);
}