
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define size 1000

int main() {
    srand(time(0));
    int *mat1[size], *mat2[size], *res[size];
    double wtime;
    int i, j, k;
    for (i = 0; i < size; i++) {
        mat1[i] = (int *)malloc(sizeof(int) * size);
        mat2[i] = (int *)malloc(sizeof(int) * size);
        res[i] = (int *)malloc(sizeof(int) * size);
    }

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++) {
            mat1[i][j] = rand() % size;
            mat2[i][j] = rand() % size;
        }
    
    wtime = omp_get_wtime();

    int temp;
    #pragma omp parallel for private(j,k) num_threads(4)
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            temp = 0;
            #pragma omp parallel for reduction(+: temp) num_threads(4)
            for (k = 0; k < size; k++)
                temp += mat1[i][k] * mat2[k][j];
            res[i][j] = temp;
        }
    }
    wtime = omp_get_wtime() - wtime;
    printf("Time taken: %g seconds\n", wtime);
    printf("Calculation Done\n");
}
