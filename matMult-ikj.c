#include <stdio.h>
#include <stdlib.h>


void matMult(int N, float **A, float **B, float **C) {

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {   //1ª iteração é necessário definir ter a matriz resultado a 0
            C[i][j] = A[i][0] * B[0][j];
        }

        for (int k = 1; k < N; k++) {
            for (int j = 0; j < N; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int N = 3;

    float** A = (float **) malloc(N*N*sizeof(float));
    float** B = (float **) malloc(N*N*sizeof(float));
    float** C = (float **) malloc(N*N*sizeof(float));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = ((float)rand()) / ((float)RAND_MAX);
            B[i][j] = 1;
        }
    }

    matMult(N, A, B, C);
}
