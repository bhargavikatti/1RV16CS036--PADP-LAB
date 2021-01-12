#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

#define POINTS_SIZE 100000000
#define CLUSTER_SIZE 4

int points[POINTS_SIZE][2];
long cluster_count[CLUSTER_SIZE];
int clusters[CLUSTER_SIZE][2] = {{75, 25}, {25, 25}, {25, 75}, {75, 75}};

void init() {
    long i;
    srand(time(0));
    for (i=0; i<CLUSTER_SIZE; i++) 
        cluster_count[i] = 0;
    for (i=0; i<POINTS_SIZE; i++) {
        points[i][0] = rand() % 100;
        points[i][1] = rand() % 100;
    }
}

double get_distance(int x1, int y1, int x2, int y2){
    int x=x2-x1, y=y2-y1;
    return (double)sqrt((x*x)+(y*y));
}


void main(){
    double t, min, cur_dist;
    init();
    long i, j;
    int cur_cluster;

    t = omp_get_wtime();
    #pragma omp parallel for private(j, min, cur_dist, cur_cluster) num_threads(2)
    for (i=0; i<POINTS_SIZE; i++) {
        min = 99999;
        for (j=0; j<CLUSTER_SIZE; j++) {
            cur_dist = get_distance(points[i][0], points[i][1], clusters[j][0], clusters[j][1]);
            if (cur_dist < min){
                min = cur_dist;
                cur_cluster = j;
            }
        }
        cluster_count[cur_cluster]++;
    }
    printf("Points in cluster 1: %ld\n", cluster_count[0]);
    printf("Points in cluster 2: %ld\n", cluster_count[1]);
    printf("Points in cluster 3: %ld\n", cluster_count[2]);
    printf("Points in cluster 4: %ld\n", cluster_count[3]);
    printf("Time taken for classification: %g\n", omp_get_wtime() - t);
}