#include <stdio.h>
#include "mpi.h"
int main(int argc, char** argv)
{
    int my_rank;  /* Identificador do processo */
    int proc_n;   /* Número de processos */
    int source;   /* Identificador do proc.origem */
    int dest;     /* Identificador do proc. destino */
    int tag = 50; /* Tag para as mensagens */
    double t1,t2; /* time stamps */
    int MSIZE;

    int *message; /* message buffer */
    MPI_Status status;  /* Status de retorno */
    
    MSIZE = atoi(argv[1]);

    message = (int *)malloc(sizeof(int)*MSIZE);

    MPI_Init (&argc , &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    if (my_rank == 0) {
    
        t1 = MPI_Wtime();  

        MPI_Send (message, MSIZE, MPI_INT, 1, tag, MPI_COMM_WORLD);
        MPI_Recv (message, MSIZE, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
        
        t2 = MPI_Wtime();
        printf("\nRound trip(s): %f\n\n", t2-t1);    
    }
    else {
    
        MPI_Recv (message, MSIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Send (message, MSIZE, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
