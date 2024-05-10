#include <stdlib.h>
#include "mpi.h"
#include "fox.h"

int main(int argc, char *argv[])
{

    int i, j, dim;
    int **localA;
    int **localB;
    int **localC;
    MPI_Init(&argc, &argv);

    GridInfo grid;
    /*initialize Grid */

    SetupGrid(&grid);
    /* Initialize matrix A & B */
    initialiseAB();
    /* calculate local matrix dimension */
    dim = N / grid.q;
    /* allocate space for the three matrices */

    localA = (int **)malloc(dim * sizeof(int *));

    localB = (int **)malloc(dim * sizeof(int *));

    localC = (int **)malloc(dim * sizeof(int *));

    for (i = 0; i < dim; i++)
    {
        *(localA + i) = (int *)malloc(dim * sizeof(int));
        *(localB + i) = (int *)malloc(dim * sizeof(int));
        *(localC + i) = (int *)malloc(dim * sizeof(int));
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

    Fox(N, &grid, localA, localB, localC);

    /* print results */
    printf("rank=%d, row=%d col=%d\n", grid.my_rank, grid.my_row, grid.my_col);
    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
        {
            // printf("localC[%d][%d]=%d ", i,j,localC[i][j]);
            printf("%d ", localC[i][j]);
        }
        printf("\n");
    }
    MPI_Finalize();
    exit(0);
}