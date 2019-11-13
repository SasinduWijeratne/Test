#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "mpi.h"

int main(int argc, char** argv) {
    int my_rank;  /* Identificador do processo */
    int proc_n;   /* Número de processos */
    int source;   /* Identificador do proc.origem */
    int dest;     /* Identificador do proc. destino */
    int tag = 50; /* Tag para as mensagens */
    double t1,t2; /* time stamps */
    int i;
    int MSIZE;
    FILE *f;
    int *message, *buffer, *tmp;

    MPI_Status status;  /* Status de retorno */
    MPI_Request request;
    int flag;

    MPI_Init (&argc , &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    MSIZE = atoi(argv[1]);

    message = (int *) malloc(sizeof(int) * MSIZE);
    buffer = (int *) malloc(sizeof(int) * MSIZE);

    message[0] = my_rank * 3;
    message[1] = my_rank * 3 + 1;
    message[2] = my_rank * 3 + 2;

    f = fopen("output.txt", "w");

    MPI_Barrier(MPI_COMM_WORLD);

    printf("My rank is %d, Total # of proc is %d, Data is %d %d %d\n", my_rank, proc_n, message[0], message[1], message[2]);

    for (i = 0; i < proc_n - 1; i++) {
        t1 = MPI_Wtime();
        MPI_Isend(message, MSIZE/proc_n, MPI_INT, (my_rank + 1) % proc_n, tag, MPI_COMM_WORLD, &request);
        MPI_Irecv(buffer, MSIZE/proc_n, MPI_INT, (my_rank + proc_n - 1) % proc_n, tag, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        t2 = MPI_Wtime();

        fprintf(f, "[Rank %d] Time for iteration %d is %f\n", my_rank, i, t2-t1);

        tmp = message;
        message = buffer;
        buffer = tmp;
        printf("My rank is %d, Total # of proc is %d, Data is %d %d %d\n", my_rank, proc_n, message[0], message[1], message[2]);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    fclose(f);

    MPI_Finalize();
    return 0;
}

