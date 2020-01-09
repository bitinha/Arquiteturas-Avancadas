#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 50

void printMatrix(int N, char c, float M[][N]) {
    printf("%c:\n", c);
    for (int i = 0; i < N; i++) {
        printf("\n");
        for (int j = 0; j < N; j++) {
            printf("%f\t", M[i][j]);
        }
    }
    printf("\n\n");
}

//https://en.wikipedia.org/wiki/In-place_matrix_transposition#Square_matrices
void transpose(int N, float M[][N]) {
    float temp;

    for (int i = 0; i < N - 2; ++i) {
        for (int j = i + 1; j < N - 1; ++j) {
            temp = M[i][j];
            M[i][j] = M[j][i];
            M[j][i] = temp;
        }
    }
}

void blockClear(int N, int i, int j, float M[][N]) {
    for (int ii = i; ii < i + BLOCK_SIZE; ii++) {
        for (int jj = j; jj < j + BLOCK_SIZE; jj++) {
            M[ii][jj] = 0;
        }
    }
}

void blockMult(int N, int i, int j, int k, float A[][N], float B[][N], float C[][N]) {
    for (int ii = i; ii < i + BLOCK_SIZE; ii++) {
        for (int jj = j; jj < j + BLOCK_SIZE; jj++) {
            for (int kk = k; kk < k + BLOCK_SIZE; kk++) {
                C[ii][jj] += A[ii][kk] * B[jj][kk];
            }
        }
    }
}

void matMultBlock(int N, float A[][N], float B[][N], float C[][N]) {
    transpose(N, B);

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

    int N = atoi(argv[1]);

    float A[N][N];
    float B[N][N];
    float C[N][N];

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = ((float)rand()) / ((float)RAND_MAX);
            B[i][j] = 1;
        }
    }

    matMultBlock(N, A, B, C);

    /* printMatrix(N, 'A', A);
    printMatrix(N, 'B', B);
    printMatrix(N, 'C', C); */
}
