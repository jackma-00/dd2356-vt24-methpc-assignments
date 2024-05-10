#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "mpi.h"
#include "fox.h"

int main(int argc, char *argv[])
{

    int i, j, dim;
    double **localA;
    double **localB;
    double **localC;
    MPI_Init(&argc, &argv);

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

    Fox(N, &grid, localA, localB, localC);

    // Expected result
    double expected[9][9] = {
        {204, 240, 276, 312, 348, 384, 420, 456, 492},
        {240, 285, 330, 375, 420, 465, 510, 555, 600},
        {276, 330, 384, 438, 492, 546, 600, 654, 708},
        {312, 375, 438, 501, 564, 627, 690, 753, 816},
        {348, 420, 492, 564, 636, 708, 780, 852, 924},
        {384, 465, 546, 627, 708, 789, 870, 951, 1032},
        {420, 510, 600, 690, 780, 870, 960, 1050, 1140},
        {456, 555, 654, 753, 852, 951, 1050, 1149, 1248},
        {492, 600, 708, 816, 924, 1032, 1140, 1248, 1356}};

    // Check if localC is as expected
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            assert(localC[i][j] == expected[i][j]);
        }
    }

    printf("Test passed\n");

        MPI_Finalize();
    exit(0);
}