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
    int i, p, iteration;
    int MSIZE;
    FILE *f;
    char filename[20];
    int *message;

    MPI_Status status;  /* Status de retorno */
    MPI_Request request;
    MPI_Request requests[16];
    int flag;

    MPI_Init (&argc , &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);
    p = proc_n - 1;

    MSIZE = atoi(argv[1]);

    message = (int *) malloc(sizeof(int) * MSIZE);

    message[0] = my_rank * 3;
    message[1] = my_rank * 3 + 1;
    message[2] = my_rank * 3 + 2;

    sprintf(filename, "output%d.txt", my_rank);
    f = fopen(filename, "w");

    MPI_Barrier(MPI_COMM_WORLD);

    if (my_rank == 0)
        printf("[Master] My rank is %d, Total # of proc is %d, Data is %d %d %d\n", my_rank, proc_n, message[0], message[1], message[2]);
    else
        printf("[Worker] My rank is %d, Total # of proc is %d, Data is %d %d %d\n", my_rank, proc_n, message[0], message[1], message[2]);

    for (iteration = 0; iteration < 5; iteration++) {
        if (my_rank == 0) {
            for (i = 1; i < proc_n; i++)
                MPI_Isend(message, MSIZE, MPI_INT, i, tag, MPI_COMM_WORLD, &request);
        } else {
            t1 = MPI_Wtime();
            MPI_Irecv(message, MSIZE, MPI_INT, 0, tag, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            t2 = MPI_Wtime();
            printf("[Worker %d] Data is %d %d %d\n", my_rank, message[0], message[1], message[2]);
            fprintf(f, "[Worker %d] Time to receive from master is %f\n", my_rank, t2-t1);
        }
        MPI_Barrier(MPI_COMM_WORLD);

        if (my_rank == 0) {
            t1 = MPI_Wtime();
            for (i = 0; i < proc_n - 1; i++)
                MPI_Irecv(&message[MSIZE/p*i], MSIZE/p, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[i]);
            for (i = 0; i < proc_n - 1; i++)
                MPI_Wait(&requests[i], &status);
            t2 = MPI_Wtime();
            fprintf(f, "[Master] Time to receive from workers is %f\n", t2-t1);
        } else {
            t1 = MPI_Wtime();
            MPI_Isend(&message[MSIZE/p*(my_rank - 1)], MSIZE/p, MPI_INT, 0, tag, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            t2 = MPI_Wtime();
            fprintf(f, "[Worker %d] Time to send to master is %f\n", my_rank, t2-t1);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    fclose(f);

    MPI_Finalize();
    return 0;
}

