#include <stdio.h>
#include <stdlib.h>


//https://en.wikipedia.org/wiki/In-place_matrix_transposition#Square_matrices
void transpose(int N, float M[][N]){

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

void matMult(int N, float A[][N], float B[][N], float C[][N]) {


    transpose(N,B);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[j][k];
            }
        }
    }
}

int main() {
    int N = 3;

    float A[N][N];
    float B[N][N];
    float C[N][N]; 

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = ((float)rand()) / ((float)RAND_MAX);
            B[i][j] = 1;
        }
    }

    matMult(N, A, B, C);
}
