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
    cout << c << endl;
    for (int i = 0; i < N; i++) {
        cout << endl;
        for (int j = 0; j < N; j++) {
            cout << M[i * N + j] << "    ";
        }
    }
    cout << "\n\n";
}

void fillMatrices(float *A, float *B, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i*N+j] = ((float)rand()) / ((float)RAND_MAX);
            B[i*N+j] = 1;
        }
    }

}

__global__ void matMultKernel_ijk(int N, float *A, float *B, float *C) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < N && j < N) {
        C[i * N + j] = 0;
        for (int k = 0; k < N; k++) {
            C[i * N + j] += A[i * N + k] * B[k * N + j];
        }
    }
}

__global__ void matMultKernel_ikj(int N, float *A, float *B, float *C) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int k = blockIdx.y * blockDim.y + threadIdx.y;

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

void matMultGPU(int N, float *A, float *B, float *C) {
    dim3 threadsPerBlock(8, 8);
    dim3 blocksPerGrid(2, 2);

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


    cudaMemcpy(dC, C, bytes, cudaMemcpyHostToDevice); /** Temporário **/

	checkCUDAError("memcpy h->d");
    // launch the kernel
	matMultKernel_ikj <<< threadsPerBlock, blocksPerGrid >>> (N, dA, dB, dC);

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
    if (argc == 1) {
        cout << "Insira o tamanho da matriz" << endl;
        exit(1);
    }

    int const N = atoi(argv[1]);

    float A[N * N];
    float B[N * N];
    float C[N * N];

    fillMatrices(A, B, N);

    /** Temporário **/
    for (int i = 0; i < N * N; i++) {
        C[i] = 0;
    }

    matMultGPU(N, A, B, C);

    printMatrix(N, 'A', A);
    printMatrix(N, 'B', B);
    printMatrix(N, 'C', C);
}
