#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 52

//https://en.wikipedia.org/wiki/In-place_matrix_transposition#Square_matrices
void transpose(int N, float *__restrict__ M) {
    float temp;

    for (int i = 0; i < N - 2; ++i) {
        // #pragma omp simd
        for (int j = i + 1; j < N - 1; ++j) {
            temp = M[i * N + j];
            M[i * N + j] = M[j * N + i];
            M[j * N + i] = temp;
        }
    }
}

void blockClear(int N, int i, int j, float *__restrict__ M) {   
    for (int ii = i; ii < i + BLOCK_SIZE; ii++) {
        for (int jj = j; jj < j + BLOCK_SIZE; jj++) {
            M[ii * N + jj] = 0;
        }
    }
}

void blockMult(int N, int i, int j, int k, float *__restrict__ A, float *__restrict__ B, float *__restrict__ C) {
    for (int ii = i; ii < i + BLOCK_SIZE; ii++) {
        for (int jj = j; jj < j + BLOCK_SIZE; jj++) {
            for (int kk = k; kk < k + BLOCK_SIZE; kk++) {
                C[ii * N + jj] += A[ii * N + kk] * B[jj * N + kk];
            }
        }
    }
}

void matMultVert(int N, float *__restrict__ A, float *__restrict__ B, float *__restrict__ C) {
    transpose(N,B);

    for (int i = 0; i < N; i+=BLOCK_SIZE) {
        for (int j = 0; j < N; j+=BLOCK_SIZE) {
            blockClear(N, i, j, C);
            for (int k = 0; k < N; k+=BLOCK_SIZE) {
                blockMult(N, i, j, k, A, B, C);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        printf("Insira o tamanho da matriz\n");
        exit(1);
    }

    int const N = atoi(argv[1]);

    float *__restrict__ A = malloc(sizeof(float) * N * N);
    float *__restrict__ B = malloc(sizeof(float) * N * N);
    float *__restrict__ C = malloc(sizeof(float) * N * N);

    float r;

    for (int i = 0; i < N; ++i) {
        r = ((float)rand()) / ((float)RAND_MAX);
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = r;
            B[i * N + j] = 1;
        }
    }

    matMultVert(N, A, B, C);
}

// #pragma omp simd
