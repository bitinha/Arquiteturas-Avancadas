#include <stdio.h>
#include <cstdlib>
#include <iostream>

using namespace std;

cudaEvent_t start, stop;

void startKernelTime (void) {
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start);
}

void stopKernelTime (void) {
	cudaEventRecord(stop);

	cudaEventSynchronize(stop);
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);

	cout << milliseconds << " ms have elapsed for the CUDA execution" << endl;
}

void checkCUDAError (const char *msg) {
	cudaError_t err = cudaGetLastError();
	if( cudaSuccess != err) {
		cerr << "Cuda error: " << msg << ", " << cudaGetErrorString( err) << endl;
		exit(-1);
	}
}

void printMatrix(int N, char c, float *M) {
    printf("%c:\n", c);
    for (int i = 0; i < N; i++) {
        printf("\n");
        for (int j = 0; j < N; j++) {
            printf("%f\t", M[i * N + j]);
        }
    }
    printf("\n\n");
}

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

__global__ void matMultKernel(int N, float *A, float *B, float *C) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < N && col < N) {
        C[row * N + col] = 0;
        for (int k = 0; k < N; k++) {
            C[row * N + col] += A[row * N + k] * B[col * N + k];
        }
    }
}

void matMultGPU(int N, float *A, float *B, float *C) {
    transpose(N,B);

    dim3 threadsPerBlock(8, 8);
    dim3 blocksPerGrid(4, 4);

    // declare variable with size of the array in bytes
    int bytes = N * N * sizeof(float);

	// pointers to the device memory
    float *dA, *dB, *dC; 

	// allocate the memory on the device
	cudaMalloc((void**) &dA, bytes);
	cudaMalloc((void**) &dB, bytes);
	cudaMalloc((void**) &dC, bytes);
    
    startKernelTime();

    checkCUDAError("mem allocation");
	// copy inputs to the device
	cudaMemcpy(dA, A, bytes, cudaMemcpyHostToDevice);
	cudaMemcpy(dB, B, bytes, cudaMemcpyHostToDevice);

	checkCUDAError("memcpy h->d");
	// launch the kernel
	matMultKernel <<< threadsPerBlock, blocksPerGrid >>> (N, dA, dB, dC);

	checkCUDAError("kernel invocation");
	// copy the output to the host
	cudaMemcpy(C, dC, bytes, cudaMemcpyDeviceToHost);
	checkCUDAError("memcpy d->h");
    
    stopKernelTime();

	// free the device memory
	cudaFree(dA); cudaFree(dB); cudaFree(dC);
	checkCUDAError("mem free");
}

int main(int argc, char const *argv[]) {
    // if (argc == 1) {
    //     printf("Insira o tamanho da matriz\n");
    //     exit(1);
    // }

    // int const N = atoi(argv[1]);
    int N = 16;

    // float *A = malloc(sizeof(float) * N * N);
    // float *B = malloc(sizeof(float) * N * N);
    // float *C = malloc(sizeof(float) * N * N);
    float A[N * N];
    float B[N * N];
    float C[N * N];

    float r;

    for (int i = 0; i < N; ++i) {
        r = ((float)rand()) / ((float)RAND_MAX);
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = r;
            B[i * N + j] = 1;
        }
    }

    matMultGPU(N, A, B, C);

    printMatrix(N, 'A', A);
    printMatrix(N, 'B', B);
    printMatrix(N, 'C', C);
}
