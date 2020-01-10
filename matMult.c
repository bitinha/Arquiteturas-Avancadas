#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 16
#define TIME_RESOLUTION 1000000 // time measuring resolution (us)

long long unsigned initial_time;
double clearcache [30000000];
struct timeval t;
int repetitions;
long long unsigned *tempos;



void clearCache (void) {
    for (unsigned i = 0; i < 30000000; ++i)
        clearcache[i] = i;
}

void printResults (void) {

    for (int i = 0; i < repetitions; ++i)
    {
        printf("T%d = %llu\n", i,tempos[i]);
    }


    long long unsigned best3[3] = {tempos[0],tempos[1],tempos[2]};

    int maior = (best3[0] > best3[1]) ? 0 : 1;
    maior = (best3[maior] > best3[2]) ? maior : 2;



    for(int i=3; i<repetitions; i++){
        if (tempos[i] < best3[maior]){

            best3[maior] = tempos[i];

            maior = (best3[0] > best3[1]) ? 0 : 1;
            maior = (best3[maior] > best3[2]) ? maior : 2;
        }
    }


    int menor = (best3[0] < best3[1]) ? 0 : 1;
    menor = (best3[menor] < best3[2]) ? menor : 2;


    long long unsigned media = (best3[0]+best3[1]+best3[2])/3;
    float tolerancia = ((float)best3[maior])/best3[menor] - 1.0;

    printf("Média dos 3 melhores tempos = %llu\nTolerância = %f%%", media, tolerancia*100);
}

void start (void) {
    gettimeofday(&t, NULL);
    initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

long long unsigned stop (int i) {
    gettimeofday(&t, NULL);
    long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;

    tempos[i] = final_time - initial_time;

    return final_time - initial_time;
}

void fillMatrices (float *A, float *B, int N) {

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i*N+j] = ((float)rand()) / ((float)RAND_MAX);
            B[i*N+j] = 1;
        }
    }

}



//https://en.wikipedia.org/wiki/In-place_matrix_transposition#Square_matrices
void transpose(float *M, int N){

    float temp;

    for (int i = 0; i < N-2; ++i)
    {
        for (int j = i+1; j < N-1; ++j)
        {
            temp = M[i*N+j];
            M[i*N+j] = M[j*N+i];
            M[j*N+i] = temp;
        }
    }

}

void printMatrix(int N, char c, float *M) {
    printf("%c:\n", c);
    for (int i = 0; i < N; i++) {
        printf("\n");
        for (int j = 0; j < N; j++) {
            printf("%f\t", M[i*N+j]);
        }
    }
    printf("\n\n");
}







void matMult_ijk(float *A, float *B, float *C, int N) {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i*N+j] = 0;
            for (int k = 0; k < N; k++) {
                C[i*N+j] += A[i*N+k] * B[k*N+j];
            }
        }
    }
}




void matMult_ikj(float *A, float *B, float *C, int N) {

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {   //1ª iteração é necessário definir ter a matriz resultado a 0
            C[i*N+j] = A[i*N+0] * B[0*N+j];
        }

        for (int k = 1; k < N; k++) {
            for (int j = 0; j < N; j++) {
                C[i*N+j] += A[i*N+k] * B[k*N+j];
            }
        }
    }
}



void matMult_jki(float *A, float *B, float *C, int N) {


    for (int j = 0; j < N; j++) {

        for (int i = 0; i < N; i++) {   //1ª iteração é necessário definir ter a matriz resultado a 0
            C[i*N+j] = A[i*N+0] * B[0*N+j];
        }

        for (int k = 1; k < N; k++) {
            for (int i = 0; i < N; i++) {
                C[i*N+j] += A[i*N+k] * B[k*N+j];
            }
        }
    }
}


void matMult_ijk_trans(float *A, float *B, float *C, int N) {


    transpose(B,N);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i*N+j] = 0;
            for (int k = 0; k < N; k++) {
                C[i*N+j] += A[i*N+k] * B[j*N+k];
            }
        }
    }
}





void matMult_jki_trans(float *A, float *B, float *C, int N) {

    transpose(A,N);
    transpose(B,N);
    // C também é transposta

    for (int j = 0; j < N; j++) {

        for (int i = 0; i < N; i++) {   //1ª iteração é necessário definir ter a matriz resultado a 0
            C[j*N+i] = A[0*N+i] * B[j*N+0];
        }

        for (int k = 1; k < N; k++) {
            for (int i = 0; i < N; i++) {
                C[j*N+i] += A[k*N+i] * B[j*N+k];
            }
        }
    }
}


void blockClear(int N, int i, int j, float *M) {
    for (int ii = i; ii < (((i + BLOCK_SIZE) < N) ? i + BLOCK_SIZE : N); ii++) {
        for (int jj = j; jj < (((j + BLOCK_SIZE) < N) ? j + BLOCK_SIZE : N); jj++) {
            M[ii*N+jj] = 0;
        }
    }
}

void blockMult(int N, int i, int j, int k, float *A, float *B, float *C) {

    // Pode-se utilizar isto para saber controlar os ciclos, mas fica muito extenso
    // int resto = N%BLOCK_SIZE;
    // int limite = N - N%BLOCK_SIZE;


    for (int ii = i; ii < (((i + BLOCK_SIZE) < N) ? i + BLOCK_SIZE : N); ii++) {
        for (int jj = j; jj < (((j + BLOCK_SIZE) < N) ? j + BLOCK_SIZE : N); jj++) {
            for (int kk = k; kk < (((k + BLOCK_SIZE) < N) ? k + BLOCK_SIZE : N); kk++) {
                C[ii*N+jj] += A[ii*N+kk] * B[jj*N+kk];
            }
        }
    }
}
void matMultBlock(float *A, float *B, float *C, int N) {
    transpose(B,N);

    for (int i = 0; i < N; i+=BLOCK_SIZE) {
        for (int j = 0; j < N; j+=BLOCK_SIZE) {
            blockClear(N, i, j, C);
            for (int k = 0; k < N; k+=BLOCK_SIZE) {
                blockMult(N, i, j, k, A, B, C);
            }
            // for (int k = limite; k < N; k++) {


            //     for (int ii = i; ii < i + resto; ii++) {
            //         for (int jj = j; jj < j + resto; jj++) {
            //             for (int kk = k; kk < k + resto; kk++) {
            //                 C[ii*N+jj] += A[ii*N+kk] * B[jj*N+kk];
            //             }
            //         }
            //     }
            

            // }
        }
    }
}



int main(int argc, char const *argv[]) {
    if (argc < 3) {
        printf("Insira a implementação desejada seguida do tamanho da matriz\n");
        exit(1);
    }

    repetitions = 8;
    tempos = malloc(sizeof(unsigned long long)*repetitions);

    void (*funcao)(float *, float *, float *, int );

    if (strcmp(argv[1],"ijk")==0) funcao = matMult_ijk;
    else if (strcmp(argv[1],"ikj")==0) funcao = matMult_ikj;
    else if (strcmp(argv[1],"jki")==0) funcao = matMult_jki;
    else if (strcmp(argv[1],"ijk_trans")==0) funcao = matMult_ijk_trans;
    else if (strcmp(argv[1],"jki_trans")==0) funcao = matMult_jki_trans;
    else if (strcmp(argv[1],"block")==0) funcao = matMultBlock;

    int N = atoi(argv[2]);


    float *A = malloc(sizeof(float) * N * N);
    float *B = malloc(sizeof(float) * N * N);
    float *C = malloc(sizeof(float) * N * N);


    fillMatrices(A,B,N);


// printMatrix(N,'A',A);
// printMatrix(N,'B',B);

    // *** Maybe some PAPI code here?

    // run your code (one of the 4 functions directly above this one)
    for (unsigned i = 0; i < repetitions; ++i) {
        clearCache();

        start();    // start time measurement

        // *** Maybe some PAPI code here?
    
        (*funcao)(A, B, C, N);
        

        // *** Maybe some PAPI code here?
        
        stop(i);   // stop time measurement

// printMatrix(N,'C',C);

    }

    free(A);
    free(B);
    free(C);

    printResults(); // writes time measurements into a file - your function and some baseline functions

    free(tempos);

    // *** Maybe some PAPI code here? And some print of the results?


}
