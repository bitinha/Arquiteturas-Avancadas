#include <cstdlib>
#include <iostream>

#define NUM_BLOCKS 16
#define NUM_THREADS_PER_BLOCK 16
#define N 16

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

void printMatrix(char c, float *M) {
    cout << c << endl;
    for (int i = 0; i < N; i++) {
        cout << endl;
        for (int j = 0; j < N; j++) {
            cout << M[i * N + j] << "    ";
        }
    }
    cout << "\n\n";
}

void fillMatrices(float *A, float *B) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i*N+j] = ((float)rand()) / ((float)RAND_MAX);
            B[i*N+j] = 1;
        }
    }

}

__global__ void matMultKernel_ijk(float *A, float *B, float *C) {
    int i = blockIdx.x;
    int j = threadIdx.x;

    if (i < N && j < N) {
        C[i * N + j] = 0;
        for (int k = 0; k < N; k++) {
            C[i * N + j] += A[i * N + k] * B[k * N + j];
        }
    }
}

__global__ void matMultKernel_ikj(float *A, float *B, float *C) {
    int i = blockIdx.x;
    int k = threadIdx.x;

    if (i < N && k < N/*  && k >= 1 */) {
        for (int j = 0; j < N; j++) {
            C[i * N + j] += A[i * N + k] * B[k * N + j];
        }
    }

    // void matMult_ikj(float *A, float *B, float *C, int N) {
    //     for (int i = 0; i < N; i++) {
    //         for (int j = 0; j < N; j++) {   //1ª iteração é necessário definir ter a matriz resultado a 0
    //             C[i*N+j] = A[i*N+0] * B[0*N+j];
    //         }
    //         for (int k = 1; k < N; k++) {
    //             for (int j = 0; j < N; j++) {
    //                 C[i*N+j] += A[i*N+k] * B[k*N+j];
    //             }
    //         }
    //     }
    // }
}

void matMultGPU(float *A, float *B, float *C) {
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


    //cudaMemcpy(dC, C, bytes, cudaMemcpyHostToDevice); /** Temporário **/

	checkCUDAError("memcpy h->d");
    // launch the kernel
	matMultKernel_ijk <<< NUM_THREADS_PER_BLOCK, NUM_BLOCKS >>> (dA, dB, dC);
	// matMultKernel_ikj <<< NUM_THREADS_PER_BLOCK, NUM_BLOCKS >>> (dA, dB, dC);

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
    float A[N * N];
    float B[N * N];
    float C[N * N];

    fillMatrices(A, B);

    // /** Temporário **/
    // for (int i = 0; i < N * N; i++) {
    //     C[i] = 0;
    // }

    matMultGPU(A, B, C);

    printMatrix('A', A);
    printMatrix('B', B);
    printMatrix('C', C);
}
