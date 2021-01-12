#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(){
    long i, count = 0, niter = 1000000;
    double x, y, z, pi, t;
    srand(time(0));

    t = omp_get_wtime();
    #pragma omp parallel for private(x, y, z) reduction(+:count) num_threads(4)
    for (i=0; i<niter; i++){
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        z = x*x + y*y;
        if (z <= 1)
            count++;
    }
    pi = (double)count/niter * 4;
    printf("Time taken: %g\n", omp_get_wtime() - t);
    printf("Value of pi: %g in %ld iterations\n", pi, niter);
}