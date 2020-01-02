#include <iostream>

using namespace std;

void matMult(float **A, float **B, float **C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int N = 512;
    float **A = new float *[N], **B = new float *[N], **C = new float *[N];

    for (int i = 0; i < N; ++i) {
        A[i] = new float[N];
        B[i] = new float[N];
        C[i] = new float[N];

        for (int j = 0; j < N; ++j) {
            A[i][j] = ((float)rand()) / ((float)RAND_MAX);
            B[i][j] = 1;
        }
    }

    matMult(A, B, C, N);
}
