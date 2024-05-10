#ifndef FOX_H
#define FOX_H

#include "mpi.h"

#define N 9 /* dimension of the input matrix */

extern int matrixA[N][N];
extern int matrixB[N][N];

typedef struct
{
    int p;             /* number of processors */
    MPI_Comm comm;     /* handle to global grid communicator */
    MPI_Comm row_comm; /* row communicator */
    MPI_Comm col_comm; /* column communicator */
    int q;             /* dimension of the grid, = sqrt(p) */
    int my_row;        /* row position of a processor in a grid */
    int my_col;        /* column position of a processor in a grid */
    int my_rank;       /* rank within the grid */
} GridInfo;

void SetupGrid(GridInfo *grid);
void matmul(double **a, double **b, double **c, int size);
void transfer_data_from_buff(double *buff, double **a, int buffsize, int row, int col);
void transfer_data_to_buff(double *buff, double **a, int buffsize, int row, int col);
void Fox(int n, GridInfo *grid, double **a, double **b, double **c);
void initialiseAB();

#endif /* FOX_H */