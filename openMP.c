#include <stdio.h>
#include <omp.h>
#include <stdlib.h>


#define M 1000
#define N 1000
#define K 1000

int main(){

    int i, j, k;
    double start_stamp, end_stamp;
    double **A, **B, **C;

    A = (double **) malloc(M * sizeof(double *));
    B = (double **) malloc(K * sizeof(double *));
    C = (double **) malloc(M * sizeof(double *));

    for (i = 0; i < M; i++) {
        A[i] = (double *) malloc(K * sizeof(double));
        C[i] = (double *) malloc(N * sizeof(double));
    }
    for (i = 0; i < K; i++) {
        B[i] = (double *) malloc(N * sizeof(double));
    }

   
    for (i = 0; i < M; i++) {
        for (j = 0; j < K; j++) {
            A[i][j] = i + j;
        }
    }
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            B[i][j] = i + j;
        }
    }

   
    start_stamp = omp_get_wtime();

    #pragma omp parallel for private(i, j, k) shared(A, B, C)
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (k = 0; k < K; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    end_stamp = omp_get_wtime();


    printf("The results of applying OpenMP: %f seconds\n", end_stamp - start_stamp);

    for (i = 0; i < M; i++) {
        free(A[i]);
        free(C[i]);
    }

    for (i = 0; i < K; i++) {
        free(B[i]);
    } 


    free(A);
    free(B);
    free(C);

    return 0;
}