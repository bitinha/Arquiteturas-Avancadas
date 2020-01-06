#include <stdio.h>
#include <stdlib.h>

//https://en.wikipedia.org/wiki/In-place_matrix_transposition#Square_matrices
void transpose(float **M, int N){

    float temp;

    for (int i = 0; i < N-2; ++i)
    {
        for (int j = i+1; j < N-1; ++j)
        {
            temp = M[i][j];
            M[i][j] = M[j][i];
            M[j][i] = temp;
        }
    }

}

void matMult(int N, float **A, float **B, float **C) {

    transpose(A,N);
    transpose(B,N);
    // C também é transposta

    for (int j = 0; j < N; j++) {

        for (int i = 0; i < N; i++) {   //1ª iteração é necessário definir ter a matriz resultado a 0
            C[j][i] = A[0][i] * B[j][0];
        }

        for (int k = 1; k < N; k++) {
            for (int i = 0; i < N; i++) {
                C[j][i] += A[k][i] * B[j][k];
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
