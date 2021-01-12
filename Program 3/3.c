#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>


long strike(int isComposite[], long start, long stride, long end) {
    long i;
    for (i=start; i<=end; i+=stride) 
        isComposite[i] = 1;
    return i;
}


void cacheUnfriendly(long n) {
    long count = 0, m = (long)sqrt(n), i;
    int *isComposite;
    isComposite = (int *)calloc(n+1, sizeof(int));

    double wtime = omp_get_wtime();
    for (i=2; i<=m; i++) 
        if (!isComposite[i]) {
            count++;
            strike(isComposite, 2*i, i, n);
        }

    for (i=m+1; i<=n; i++)
        if (!isComposite[i])
            count++;
    wtime = omp_get_wtime() - wtime;
    printf("Number of primes: %ld in %g seconds\n", count, wtime);
}


void cacheFriendly(long n) {
    long count = 0, m = (long)sqrt(n), i, w;
    int *isComposite;
    isComposite = (int *)calloc(n+1, sizeof(int));

    long *factor, *strike_start;
    factor = (long *)malloc(sizeof(long) * n);
    strike_start = (long *)malloc(sizeof(long) * n);
    long n_factor = 0;

    double wtime = omp_get_wtime();
    for (i=2; i<=m; i++) 
        if (!isComposite[i]) {
            count++;
            strike_start[n_factor] = strike(isComposite, 2*i, i, m);
            factor[n_factor++] = i;
        }

    long limit;
    for (w=m+1; w<=n; w+=m) {
        limit = fmin(w+m-1, n);
        #pragma omp parallel for
        for (long i=0; i<n_factor; i++)
            strike_start[i] = strike(isComposite, strike_start[i], factor[i], limit);
        
        #pragma omp parallel for reduction(+:count)
        for (i=w; i<=limit; i++)
            if (!isComposite[i])
                count++;
    }
    wtime = omp_get_wtime() - wtime;
    printf("Number of primes: %ld in %g seconds\n", count, wtime);
}



int main() {
    cacheUnfriendly(100);
    cacheFriendly(1000);
}
