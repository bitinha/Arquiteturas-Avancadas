#include <stdio.h>
#include <stdlib.h>


void matMult(int N, float A[][N], float B[][N], float C[][N]) {

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

int main(int argc, char const *argv[]) {

    if (argv == 1){
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

    matMult(N, A, B, C);
}
