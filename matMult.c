#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include "papi.h"

#define BLOCK_SIZE 16
#define TIME_RESOLUTION 1000000 // time measuring resolution (us)

long long unsigned initial_time;
double clearcache [100000000];
struct timeval t;
int repetitions;
long long unsigned *tempos;

int validar(float *C, int N){
    for(int i=0; i<N; i++){
        for (int j = 0; j < N; ++j)
        {
            if(C[i*N] != C[i*N+j]){
                printf("%f != %f\n", C[i*N],C[i*N+j]);
                return 1;
            }
        }    
    }
    return 0;
}

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

    printf("Média dos 3 melhores tempos = %llu\nTolerância = %f%%\n", media, tolerancia*100);
}

void printPAPI(long long *values, int num_events, const char *tipo, int i){

    printf("Medições da iteração %d:\n\n",i);

    if (strcmp(tipo,"pm1")==0){

        printf("Miss L1=%lld\nLoad Instrucions=%lld\nStore Instrucions=%lld\n", values[0], values[1], values[2]);
        printf("Miss Rate L1 = %f\n", (double)values[0]/(values[1]+values[2]));
    }
    else if (strcmp(tipo,"pm23")==0){
        printf("Miss L1=%lld\nMiss L2=%lld\nMiss L3=%lld\n", values[0], values[1], values[2]);
        printf("Miss Rate L2 = %f\nMiss Rate L3 = %f\n", (double)values[1]/values[0], (double)values[2]/values[1]);
    }

    printf("\n\n\n");
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

    for (int i = 0; i < N-1; ++i)
    {
        for (int j = i+1; j < N; ++j)
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
    transpose(B,N);

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

    transpose(A,N);
    transpose(B,N);
    transpose(C,N);
}


void blockClear(int N, int i, int j, float *M) {
    for (int ii = i; ii < (((i + BLOCK_SIZE) < N) ? i + BLOCK_SIZE : N); ii++) {
        for (int jj = j; jj < (((j + BLOCK_SIZE) < N) ? j + BLOCK_SIZE : N); jj++) {
            M[ii*N+jj] = 0;
        }
    }
}

void blockMultijk(int N, int i, int j, int k, float * __restrict__ A, float * __restrict__ B, float * __restrict__ C) {


    // Pode-se utilizar isto para saber controlar os ciclos, mas fica muito extenso
    int resto = N%BLOCK_SIZE;
    int limite = N - N%BLOCK_SIZE;

    if ( k + BLOCK_SIZE < N) {
        for (int ii = i; ii < (((i + BLOCK_SIZE) < N) ? i + BLOCK_SIZE : N); ii++) {
            for (int jj = j; jj < (((j + BLOCK_SIZE) < N) ? j + BLOCK_SIZE : N); jj++) {
                //unroll do loop para que possa ser vetorizado o anterior
                C[ii*N+jj] += A[ii*N] * B[jj*N];
                C[ii*N+jj] += A[ii*N+1] * B[jj*N+1];
                C[ii*N+jj] += A[ii*N+2] * B[jj*N+2];
                C[ii*N+jj] += A[ii*N+3] * B[jj*N+3];
                C[ii*N+jj] += A[ii*N+4] * B[jj*N+4];
                C[ii*N+jj] += A[ii*N+5] * B[jj*N+5];
                C[ii*N+jj] += A[ii*N+6] * B[jj*N+6];
                C[ii*N+jj] += A[ii*N+7] * B[jj*N+7];
                C[ii*N+jj] += A[ii*N+8] * B[jj*N+8];
                C[ii*N+jj] += A[ii*N+9] * B[jj*N+9];
                C[ii*N+jj] += A[ii*N+10] * B[jj*N+10];
                C[ii*N+jj] += A[ii*N+11] * B[jj*N+11];
                C[ii*N+jj] += A[ii*N+12] * B[jj*N+12];
                C[ii*N+jj] += A[ii*N+13] * B[jj*N+13];
                C[ii*N+jj] += A[ii*N+14] * B[jj*N+14];
                C[ii*N+jj] += A[ii*N+15] * B[jj*N+15];
            }
        }
    }
    else{
    for (int ii = i; ii < (((i + BLOCK_SIZE) < N) ? i + BLOCK_SIZE : N); ii++) {
        for (int jj = j; jj < (((j + BLOCK_SIZE) < N) ? j + BLOCK_SIZE : N); jj++) {
                for (int kk = k; kk <  N; kk++) {
                    C[ii*N+jj] += A[ii*N+kk] * B[jj*N+kk];
                }
            }
        }
    }

}

void matMultBlockijk(float *A, float *B, float *C, int N) {
    transpose(B,N);

    for (int i = 0; i < N; i+=BLOCK_SIZE) {
        for (int j = 0; j < N; j+=BLOCK_SIZE) {
            blockClear(N, i, j, C);
            for (int k = 0; k < N; k+=BLOCK_SIZE) {
                blockMultijk(N, i, j, k, A, B, C);
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
    transpose(B,N);

}

void matMultBlockijkomp(float *A, float *B, float *C, int N) {
    transpose(B,N);

    #pragma omp parallel for
    for (int i = 0; i < N; i+=BLOCK_SIZE) {
        for (int j = 0; j < N; j+=BLOCK_SIZE) {
            blockClear(N, i, j, C);
            for (int k = 0; k < N; k+=BLOCK_SIZE) {
                blockMultijk(N, i, j, k, A, B, C);
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
    transpose(B,N);

}

void blockMultikj(int N, int i, int j, int k, float *A, float *B, float *C) {



    for (int ii = i; ii < (((i + BLOCK_SIZE) < N) ? i + BLOCK_SIZE : N); ii++) {
        for (int kk = k; kk < (((k + BLOCK_SIZE) < N) ? k + BLOCK_SIZE : N); kk++) {
            for (int jj = j; jj < (((j + BLOCK_SIZE) < N) ? j + BLOCK_SIZE : N); jj++) {
                C[ii*N+jj] += A[ii*N+kk] * B[kk*N+jj];
            }
        }
    }

}
void matMultBlockikj(float *A, float *B, float *C, int N) {

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            C[i*N+j]=0;
        }
    }

    for (int i = 0; i < N; i+=BLOCK_SIZE) {
        for (int k = 0; k < N; k+=BLOCK_SIZE) {
            for (int j = 0; j < N; j+=BLOCK_SIZE) {
                blockMultikj(N, i, j, k, A, B, C);
            }
        }
    }
}

void blockMultjki(int N, int i, int j, int k, float *A, float *B, float *C) {



    for (int jj = j; jj < (((j + BLOCK_SIZE) < N) ? j + BLOCK_SIZE : N); jj++) {
        for (int kk = k; kk < (((k + BLOCK_SIZE) < N) ? k + BLOCK_SIZE : N); kk++) {
            for (int ii = i; ii < (((i + BLOCK_SIZE) < N) ? i + BLOCK_SIZE : N); ii++) {
                C[jj*N+ii] += A[kk*N+ii] * B[jj*N+kk];
            }
        }
    }

}
void matMultBlockjki(float *A, float *B, float *C, int N) {

    transpose(A,N);
    transpose(B,N);

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            C[i*N+j]=0;
        }
    }

    for (int j = 0; j < N; j+=BLOCK_SIZE) {
        for (int k = 0; k < N; k+=BLOCK_SIZE) {
            for (int i = 0; i < N; i+=BLOCK_SIZE) {
                blockMultjki(N, i, j, k, A, B, C);
            }
        }
    }

    transpose(A,N);
    transpose(B,N);
    transpose(C,N);
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
    else if (strcmp(argv[1],"ijk_block")==0) funcao = matMultBlockijk;
    else if (strcmp(argv[1],"ikj_block")==0) funcao = matMultBlockikj;
    else if (strcmp(argv[1],"jki_block")==0) funcao = matMultBlockjki;
    else if (strcmp(argv[1],"ijk_omp")==0) funcao = matMultBlockijkomp;
    else if (strcmp(argv[1],"ikj_omp")==0) funcao = matMultBlockijkomp;
    else if (strcmp(argv[1],"jki_omp")==0) funcao = matMultBlockijkomp;
    else {
        printf("Insira a implementação desejada:\n");

        char *implementacoes[11] = {"ijk","ikj","jki","ijk_trans","jki_trans","ijk_block","ikj_block","jki_block","ijk_omp","ikj_omp","jki_omp"};
        for(int i = 0; i<11;i++){
            printf(" - %s\n", implementacoes[i]);
        }
        exit(1);
    }

    int N = atoi(argv[2]);


    // float *A = malloc(sizeof(float) * N * N);
    // float *B = malloc(sizeof(float) * N * N);
    // float *C = malloc(sizeof(float) * N * N);

    float *A;
    float *B;
    float *C;

    posix_memalign((void **)&A, 256, sizeof(float) * N * N);
    posix_memalign((void **)&B, 256, sizeof(float) * N * N);
    posix_memalign((void **)&C, 256, sizeof(float) * N * N);


    fillMatrices(A,B,N);
    // printMatrix(N,'A',A);
    // printMatrix(N,'B',B);

    char papi = 0;
    int num_events = 3;
    long long values[num_events];
    int events[num_events];

    if (argc > 3){
        if (strcmp(argv[3],"pm1")==0){
            papi = 1;
            events[0] = PAPI_L1_DCM;
            events[1] = PAPI_LD_INS;
            events[2] = PAPI_SR_INS;
        }
        else if (strcmp(argv[3],"pm23")==0){
            papi = 1;
            events[0] = PAPI_L1_DCM;
            events[1] = PAPI_L2_DCM;
            events[2] = PAPI_L3_TCM;
        }
    }

    if (papi == 0){
        for (unsigned i = 0; i < repetitions; ++i) {
            clearCache();

            start();    // start time measurement

            (*funcao)(A, B, C, N);
            
            stop(i);   // stop time measurement

            // printMatrix(N,'C',C);

        }
    }else{

        for (unsigned i = 0; i < repetitions; ++i) {
            clearCache();

            start();    // start time measurement

            PAPI_start_counters(events,num_events);

            (*funcao)(A, B, C, N);
            
            PAPI_stop_counters(values,num_events);
            
            stop(i);   // stop time measurement

            printPAPI(values,num_events,argv[3],i);

        }
    }

    if(validar(C,N) == 1){
        printf("Erro na validação\n");
    }else{
        printf("Validação correta\n");
    }

    free(A);
    free(B);
    free(C);

    printResults(); // writes time measurements into a file - your function and some baseline functions

    free(tempos);

    // *** Maybe some PAPI code here? And some print of the results?


}
