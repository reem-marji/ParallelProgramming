#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define M 1000
#define N 1000
#define K 1000
#define NUM_THREADS 4

double A[M][N], B[N][K], C[M][K];
pthread_t threads[NUM_THREADS];
int thread_args[NUM_THREADS];

void *matrixMultiplication(void *thread_arg) {

    int tid = *((int*) thread_arg);
    int i, j, k;
    double sum;

    int chunck_size = M / NUM_THREADS;
    int start = tid * chunck_size;
    int end;

    if(tid == NUM_THREADS - 1){
	end = M; }
    else {
	end = start + chunck_size; }


    for (i = start; i < end; i++) {
        for (j = 0; j < K; j++) {
            sum = 0.0;
            for (k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    pthread_exit(NULL);
}

int main() {

    int i, j;
    void *status;


    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i + j;
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < K; j++) {
            B[i][j] = i + j;
        }
    }

    time_t start = time(NULL);
    
    for (i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, matrixMultiplication, (void*) &thread_args[i]);
    }

  
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }

    printf("The result of applying pthreads: %.6f\n", (double)(time(NULL)-start));

    return 0;
}
