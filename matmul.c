#include <stdio.h>
#include <malloc.h>
#include <time.h>

#define size 5000
#define IDX(x,y) ((x)*size+(y))

int main() {
    int *A, *B, *C;
    int s;

    A = (int *)malloc(sizeof(int)*size*size);
    B = (int *)malloc(sizeof(int)*size*size);
    C = (int *)malloc(sizeof(int)*size*size);

    for (int i = 0; i < size*size; i++) {
        A[i] = 1;
        B[i] = 2;
    }

    struct timespec time1, time2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            s = 0;
            for (int k = 0; k < size; k++)
                s += A[IDX(i,k)]*B[IDX(k,j)];
            C[IDX(i,j)] = s;
        }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    struct timespec temp;
    if (time2.tv_nsec - time1.tv_nsec < 0) {
        temp.tv_sec = time2.tv_sec - time1.tv_sec - 1;
        temp.tv_nsec = 1000000000 + time2.tv_nsec - time1.tv_nsec;
    } else {
        temp.tv_sec = time2.tv_sec - time1.tv_sec;
        temp.tv_nsec = time2.tv_nsec - time1.tv_nsec;        
    }

    printf("Time: %ld.%ld\n", temp.tv_sec, temp.tv_nsec);

    for (int i = 0; i < 32; i++)
        printf("C[%d]=%d ", i, C[i]);
    printf("\n");
    return 0;
}