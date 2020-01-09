#include <stdio.h>
#include <stdlib.h>


//https://en.wikipedia.org/wiki/In-place_matrix_transposition#Square_matrices
void transpose(int N, float *__restrict__ M) {
    float temp;

    for (int i = 0; i < N - 2; ++i) {
        for (int j = i + 1; j < N - 1; ++j) {
            temp = M[i * N + j];
            M[i * N + j] = M[j * N + i];
            M[j * N + i] = temp;
        }
    }
}

void matMultVert(int N, float const *__restrict__ A, float *__restrict__ B, float *__restrict__ C) {
    transpose(N,B);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i * N + j] = 0;
            for (int k = 0; k < N; k++) {
                C[i * N + j] += A[i * N + k] * B[j * N + k];
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
