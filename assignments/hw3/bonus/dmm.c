/* fox.c -- uses Fox's algorithm to multiply two square matrices
 * modified from Pacheco's example - bds 03Feb
 *
 * Input:
 *     n: global order of matrices
 *     A,B: the factor matrices
 * Output:
 *     C: the product matrix
 *
 * Notes:  
 *     1.  Assumes the number of processes is a perfect square
 *     2.  The array member of the matrices is statically allocated
 *     3.  Assumes the global order of the matrices is evenly
 *         divisible by sqrt(p).
 *
 * See Pacheco, Chap 7, pp. 113 & ff and pp. 125 & ff in PPMPI
 */
#include <stdio.h>
#include "mpi.h"
#include <math.h>
#include <stdlib.h>

#define MAX 4000000 
typedef struct {
    int     n_bar;
#define Order(A) ((A)->n_bar)
    float  entries[MAX];
#define Entry(A,i,j) (*(((A)->entries) + ((A)->n_bar)*(i) + (j)))
} LOCAL_MATRIX_T;

/* Function Declarations */
LOCAL_MATRIX_T* Local_matrix_allocate(int n_bar);
void            Set_matrix( LOCAL_MATRIX_T*  local_A, int r, int q);
void            Set_to_zero(LOCAL_MATRIX_T* local_A);
void            Local_matrix_multiply( LOCAL_MATRIX_T* local_A,
                    LOCAL_MATRIX_T* local_B, LOCAL_MATRIX_T* local_C);
void            Build_matrix_type( LOCAL_MATRIX_T* local_A);
void Print_block(char* title, LOCAL_MATRIX_T* block);

MPI_Datatype    local_matrix_mpi_t;

void 		Fox( LOCAL_MATRIX_T* local_A, LOCAL_MATRIX_T* local_B, 
		    LOCAL_MATRIX_T* local_C, MPI_Comm comm); 

/*********************************************************/
int      pproc = 0; // global for  debug printing
main(int argc, char* argv[]) {
    int              p; // num procs
    int              q; // sqrt of p, num procs per row and per col
    int              r; // my rank
    int              i,j; // indices into a block
    LOCAL_MATRIX_T*  local_A;
    LOCAL_MATRIX_T*  local_B;
    LOCAL_MATRIX_T*  local_C;
    int              n; // matrix order (matrices are n by n)
    int              n_bar; // block order (block is n_bar by n_bar)

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    q = (int) sqrt((double) p);

    if (r == 0) n = atoi(argv[1]); 
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    n_bar = n/q;
    if (r == pproc) printf("n=%d\n", n); 

    local_A = Local_matrix_allocate(n_bar);
    Order(local_A) = n_bar;
    Set_matrix(local_A, r, q);
    // we'll just print pproc's block for a partial correctness check
    if (r == pproc) Print_block("A starts with this block", local_A);

    local_B = Local_matrix_allocate(n_bar);
    Order(local_B) = n_bar;
    Set_matrix(local_B, r, q);
    // we'll just print pproc's block for a partial correctness check
    if (r == pproc) Print_block("B starts with this block", local_B);

    local_C = Local_matrix_allocate(n_bar);
    Order(local_C) = n_bar;
    Fox( local_A, local_B, local_C, MPI_COMM_WORLD); 

    // we'll just print pproc's block for a partial correctness check
    if (r == pproc) Print_block("C starts with this block", local_C);
   
    MPI_Finalize();
}  /* main */


/*********************************************************/
void Fox(
        LOCAL_MATRIX_T*  local_A   /* in  */,
        LOCAL_MATRIX_T*  local_B   /* in  */,
        LOCAL_MATRIX_T*  local_C   /* out */,
        MPI_Comm         comm      /* in, communicator */) {

    LOCAL_MATRIX_T*  temp_A; /* Storage for the sub-    */
                             /* matrix of A used during */ 
                             /* the current stage       */
    LOCAL_MATRIX_T*  temp_B; /* Storage for the sub-    */
                             /* matrix of B during rotation of B blocks */ 
    int              s; // index of current stage
    int              tag = 0;
    MPI_Status       status;
    int              p; // numprocs
    int              q; // procs per row, per col
    int              r; // my rank
    int              i; // r / q, my row index 
    int              j; // r % q, my col index 
    int              k;
    float*           temp_old_B_entries;
    float*           temp_new_B_entries;

    MPI_Comm_rank(comm, &r);
    MPI_Comm_size(comm, &p);
    q = (int) sqrt((double) p);
    i = r / q; 
    j = r % q;

    Set_to_zero(local_C);

    /* Set aside storage for the broadcast block of A */
    temp_A = Local_matrix_allocate(Order(local_A));
    Order(temp_A) = Order(local_A);
    Build_matrix_type(temp_A); // builds local_matrix_mpi_t

    for (s = 0; s < q; ++s) {
        //printf("proc %d = (%d, %d) enter stage %d\n", r, i, j, s); fflush(stdout);
	/// 1. Bcast A[i,i+s] to all in row i.
        if (j == ((i + s)%q)) { // I'm the bcast sender
            for (k = 0; k < q; ++k)
                if (k != j)  
                    MPI_Send(local_A, 1, local_matrix_mpi_t,
                         /* destination */ i*q + k, tag, comm);

            /// C[i,j] += A[i,i+s]*B[i+s,j]
            Local_matrix_multiply(local_A, local_B, local_C);

        } else { // recv the A[i,i+s] block
            MPI_Recv(temp_A, 1, local_matrix_mpi_t,
                /* source */ i*q + (i + s)%q, tag, comm, &status);

            /// C[i,j] += A[i,i+s]*B[i+s,j]
            Local_matrix_multiply(temp_A, local_B, local_C);
        }

        //printf("proc %d = (%d, %d) multiplied in stage %d\n", r, i, j, s); fflush(stdout);
        /// 3. Calculate addresses for circular shift of B 
	/// proc r = i*q + j now has B[i+s, j], 
	/// but it wants B[i+s+1,j] in preparation for next stage.

        if (0 == (i % 2)) {
            MPI_Send(local_B, 1, local_matrix_mpi_t,
                /* dest below */ ((i - 1 + q)%q)*q + j, tag, comm);
            MPI_Recv(local_B, 1, local_matrix_mpi_t,
                /* source above */ ((i + 1)%q)*q + j, tag, comm, &status);
        } else {
            temp_B = Local_matrix_allocate(Order(local_B));
            Order(temp_B) = Order(local_B);
	    for (k = 0; k < Order(local_B); ++k) temp_B->entries[k] = local_B->entries[k];

            MPI_Recv(local_B, 1, local_matrix_mpi_t,
                /* source above */ ((i + 1)%q)*q + j, tag, comm, &status);

            MPI_Send(temp_B, 1, local_matrix_mpi_t,
                /* dest below */ ((i - 1 + q)%q)*q + j, tag, comm);
        }
/*
        // works on global comm?   -- no
        MPI_Sendrecv_replace(local_B, 1, local_matrix_mpi_t,
            i*q + dest_col, tag, i*q + source_col, tag, comm, &status);
*/
    } /* for s */
    
} /* Fox */


/*********************************************************/
LOCAL_MATRIX_T* Local_matrix_allocate(int local_order) {
    LOCAL_MATRIX_T* temp;
  
    temp = (LOCAL_MATRIX_T*) malloc(sizeof(LOCAL_MATRIX_T));
    return temp;
}  /* Local_matrix_allocate */


/*********************************************************/
/* Set local input matrix block
 * For now, just a block of the hilbert matrix defined by h[r, c] = 1/(r + c + 1)
 * where r and c are the indices of the global matrix.
 */
void Set_matrix(
         LOCAL_MATRIX_T*  local_A  /* out */,
         int              r        /* in  */, 
         int              q        /* in  */) {
    int I = r / q; // indices of block
    int J = r % q;
    int i, j;      // local indices within block

// we'll just us the Hilbert matrix for this example
    for (i = 0; i < Order(local_A); ++i)
        for (j = 0; j < Order(local_A); ++j)
            Entry(local_A, i, j) = 1/((double)((I+J)*Order(local_A) + i + j + 1));
}  /* Set_matrix */


/*********************************************************/
void Set_to_zero(
         LOCAL_MATRIX_T*  local_A  /* out */) {

    int i, j;

    for (i = 0; i < Order(local_A); i++)
        for (j = 0; j < Order(local_A); j++)
            Entry(local_A,i,j) = 0.0;

}  /* Set_to_zero */


/*********************************************************/
void Build_matrix_type(
         LOCAL_MATRIX_T*  local_A  /* in */) {
    MPI_Datatype  temp_mpi_t;
    int           block_lengths[2];
    MPI_Aint      displacements[2];
    MPI_Datatype  typelist[2];
    MPI_Aint      start_address;
    MPI_Aint      address;

    MPI_Type_contiguous(Order(local_A)*Order(local_A), 
        MPI_FLOAT, &temp_mpi_t);

    block_lengths[0] = block_lengths[1] = 1;
   
    typelist[0] = MPI_INT;
    typelist[1] = temp_mpi_t;

    MPI_Address(local_A, &start_address);
    MPI_Address(&(local_A->n_bar), &address);
    displacements[0] = address - start_address;
    
    MPI_Address(local_A->entries, &address);
    displacements[1] = address - start_address;

    MPI_Type_struct(2, block_lengths, displacements,
        typelist, &local_matrix_mpi_t);
    MPI_Type_commit(&local_matrix_mpi_t); 
}  /* Build_matrix_type */


/*********************************************************/
void Local_matrix_multiply(
         LOCAL_MATRIX_T*  local_A  /* in  */,
         LOCAL_MATRIX_T*  local_B  /* in  */, 
         LOCAL_MATRIX_T*  local_C  /* out */) {
    int i, j, k;

    for (i = 0; i < Order(local_A); i++)
        for (j = 0; j < Order(local_A); j++)
            for (k = 0; k < Order(local_B); k++)
                Entry(local_C,i,j) = Entry(local_C,i,j) 
                    + Entry(local_A,i,k)*Entry(local_B,k,j);

}  /* Local_matrix_multiply */


/*********************************************************/
void Print_block(
	char* title  /* in */,
        LOCAL_MATRIX_T*  block  /* in  */) {
    int i, j;
    printf("%s\n", title);
    for (i = 0; i < block->n_bar; ++i) {
        for (j = 0; j < block->n_bar; ++j) printf("%6.3f ", Entry(block,i,j));
        printf("\n"); fflush(stdout);
    }
}