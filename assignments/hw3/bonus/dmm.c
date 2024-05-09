#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_SIZE 6

double** allocate_matrix(int rows, int cols) {
    double** matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)malloc(cols * sizeof(double));
    }
    return matrix;
}

void free_matrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void print_matrix(double** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

void initialize_matrix(double** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
    }
}

void matrix_multiply(double** A, double** B, double** C, int block_size) {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < block_size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, comm_size;
    int block_size;
    double** A;
    double** B;
    double** C;
    double** local_A;
    double** local_B;
    double** local_C;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    block_size = MATRIX_SIZE / comm_size;

    if (MATRIX_SIZE % comm_size != 0) {
        if (rank == 0) {
            printf("Number of processes must divide the matrix size.\n");
        }
        MPI_Finalize();
        exit(1);
    }

    A = allocate_matrix(MATRIX_SIZE, MATRIX_SIZE);
    B = allocate_matrix(MATRIX_SIZE, MATRIX_SIZE);
    C = allocate_matrix(MATRIX_SIZE, MATRIX_SIZE);
    local_A = allocate_matrix(block_size, MATRIX_SIZE);
    local_B = allocate_matrix(MATRIX_SIZE, block_size);
    local_C = allocate_matrix(block_size, block_size);

    if (rank == 0) {
        // Initialize matrices A and B
        initialize_matrix(A, MATRIX_SIZE, MATRIX_SIZE);
        initialize_matrix(B, MATRIX_SIZE, MATRIX_SIZE);
    }

    // Scatter matrices A and B to all processes
    MPI_Scatter(A[0], MATRIX_SIZE * block_size, MPI_DOUBLE, local_A[0], MATRIX_SIZE * block_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B[0], MATRIX_SIZE * block_size, MPI_DOUBLE, local_B[0], MATRIX_SIZE * block_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform local matrix multiplication
    matrix_multiply(local_A, local_B, local_C, block_size);

    // Gather local results from all processes
    MPI_Gather(local_C[0], MATRIX_SIZE * block_size, MPI_DOUBLE, C[0], MATRIX_SIZE * block_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Print the result in process 0
    if (rank == 0) {
        printf("Matrix A:\n");
        print_matrix(A, MATRIX_SIZE, MATRIX_SIZE);
        printf("\nMatrix B:\n");
        print_matrix(B, MATRIX_SIZE, MATRIX_SIZE);
        printf("\nMatrix C:\n");
        print_matrix(C, MATRIX_SIZE, MATRIX_SIZE);
    }

    // Free memory
    free_matrix(A, MATRIX_SIZE);
    free_matrix(B, MATRIX_SIZE);
    free_matrix(C, MATRIX_SIZE);
    free_matrix(local_A, block_size);
    free_matrix(local_B, MATRIX_SIZE);
    free_matrix(local_C, block_size);

    MPI_Finalize();
    return 0;
}
