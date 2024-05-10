#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "mpi.h"

#define N 9 /* dimension of the input matrix */

int matrixA[N][N];
int matrixB[N][N];

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

void SetupGrid(GridInfo *grid)
{
    int old_rank;
    int dimensions[2];
    int wrap_around[2];
    int coordinates[2];
    int free_coords[2];

    /* get the overall information before overlaying cart_grid */

    MPI_Comm_size(MPI_COMM_WORLD, &(grid->p));
    MPI_Comm_rank(MPI_COMM_WORLD, &old_rank);

    /* Assumption: p is a perfect square */
    grid->q = (int)sqrt((double)grid->p);
    /* set the dimensions */
    dimensions[0] = dimensions[1] = grid->q;

    /* we want a torus on the second dimension, so set it appropriately */

    wrap_around[0] = 0;
    wrap_around[1] = 1;

    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, wrap_around, 1, &(grid->comm));
    /* since we have set reorder to true, this might have changed the ranks */
    MPI_Comm_rank(grid->comm, &(grid->my_rank));
    /* get the cartesian coordinates for the current process */
    MPI_Cart_coords(grid->comm, grid->my_rank, 2, coordinates);
    /* set the coordinate values for the current coordinate */
    grid->my_row = coordinates[0];
    grid->my_col = coordinates[1];

    /* create row communicators */
    free_coords[0] = 0;
    free_coords[1] = 1; /* row is gonna vary */
    MPI_Cart_sub(grid->comm, free_coords, &(grid->row_comm));

    /* create column communicators */
    free_coords[0] = 1;
    free_coords[1] = 0; /* row is gonna vary */
    MPI_Cart_sub(grid->comm, free_coords, &(grid->col_comm));
}

/* normal matrix multiplication stuff */

void matmul(int **a, int **b, int **c, int size)
{
    int i, j, k;

    int **temp = (int **)malloc(size * sizeof(int *));
    for (i = 0; i < size; i++)
        *(temp + i) = (int *)malloc(size * sizeof(int));

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            temp[i][j] = 0;
            for (k = 0; k < size; k++)
            {
                temp[i][j] = temp[i][j] + (a[i][k] * b[k][j]);
            }
        }
    }

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            c[i][j] += temp[i][j];
}
void transfer_data_from_buff(int *buff, int **a, int buffsize, int row, int col)
{

    if (buffsize != row * col)
    {
        printf("transfer_data_from_buf: buffer size does not match matrix size!\n");
        exit(1);
    }
    int count = 0, i, j;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            a[i][j] = buff[count];
            count++;
        }
    }
}

void transfer_data_to_buff(int *buff, int **a, int buffsize, int row, int col)
{

    if (buffsize != row * col)
    {
        printf("transfer_data_to_buf: buffer size does not match matrix size!");
        exit(1);
    }
    int count = 0, i, j;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            buff[count] = a[i][j];
            count++;
        }
    }
}

void Fox(int n, GridInfo *grid, int **a, int **b, int **c)
{
    int **tempa;
    int *buff; /* buffer for Bcast & send_recv */
    int stage;
    int root;
    int submat_dim; /* = n/q */
    int source;
    int dest;
    int i;
    MPI_Status status;

    submat_dim = n / grid->q;

    /* Initialize tempa */
    tempa = (int **)malloc(submat_dim * sizeof(int *));
    for (i = 0; i < submat_dim; i++)
        *(tempa + i) = (int *)malloc(submat_dim * sizeof(int));
    /* initialize buffer */
    buff = (int *)malloc(submat_dim * submat_dim * sizeof(int));

    /* we are gonna shift the elements of matrix b upwards with the column fixed */
    source = (grid->my_row + 1) % grid->q;         /* pick the emmediately lower element */
    dest = (grid->my_row + grid->q - 1) % grid->q; /* move current element to immediately upper row */

    for (stage = 0; stage < grid->q; stage++)
    {
        root = (grid->my_col + stage) % grid->q;
        if (root == grid->my_col)
        {
            transfer_data_to_buff(buff, a, submat_dim * submat_dim, submat_dim, submat_dim);
            MPI_Bcast(buff, submat_dim * submat_dim, MPI_INT, root, grid->row_comm);
            transfer_data_from_buff(buff, a, submat_dim * submat_dim, submat_dim, submat_dim);

            matmul(a, b, c, submat_dim);
        }
        else
        {
            transfer_data_to_buff(buff, tempa, submat_dim * submat_dim, submat_dim, submat_dim);
            MPI_Bcast(buff, submat_dim * submat_dim, MPI_INT, root, grid->row_comm);
            transfer_data_from_buff(buff, tempa, submat_dim * submat_dim, submat_dim, submat_dim);

            matmul(tempa, b, c, submat_dim);
        }
        transfer_data_to_buff(buff, b, submat_dim * submat_dim, submat_dim, submat_dim);
        MPI_Sendrecv_replace(buff, submat_dim * submat_dim, MPI_INT, dest, 0, source, 0, grid->col_comm, &status);
        transfer_data_from_buff(buff, b, submat_dim * submat_dim, submat_dim, submat_dim);
    }
}

void initialiseAB()
{
    int i, j;
    /* *****************************************************************************************
    Initialize the input matrix
    Note: This initalization is deterministic & hence is done by every process in the same way
          I wanted to design a fully distributed program, hence I took this strategy.
          A better strategy could have been to let master alone initialize the matrices & then
          send the slaves their local copy only
    *******************************************************************************************/
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            matrixA[i][j] = i + j;
            matrixB[i][j] = i + j;
        }
    }
}