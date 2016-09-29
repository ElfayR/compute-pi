#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "computepi.h"

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0
#define N_STEP 1000
#define TEST_COUNT 50
//#define M_PI = acos(-1.0)

void compute_ci(double* input, int size)
{
    //caculate mean
    double mean_value, sd, min_ci, max_ci, temp = 0;
    int count = 0;

    for (int i = 0; i < size; i++) {
        mean_value += input[i];
    }
    mean_value = mean_value / size;
    //caculate sd
    for (int j = 0; j < size; j++) {
        temp = (input[j] - mean_value);
        sd += temp * temp;
    }
    sd = sqrt((sd / size));

    min_ci = mean_value - 2 * sd;
    max_ci = mean_value + 2 * sd;
    temp = 0;

    for (int k = 0; k < size; k++) {
        if ((input[k] < max_ci) && (input[k] > min_ci)) {
            temp += input[k];
            count++;
        }
    }
    printf("%lf ",(temp / count));
    //printf("%lf %lf %lf\n",mean_value,sd,(temp / count));

}

int main(int argc, char const *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};
    double array_ci[TEST_COUNT];

    if (argc < 2) return -1;

    int N = atoi(argv[1]);
    int i, loop = TEST_COUNT;
    int j = 1;
    for (j = 10000; j < N; j = j + N_STEP) {
        printf("%d ", j);

        // Baseline
        for(i = 0; i < loop; i++) {
            clock_gettime(CLOCK_ID, &start);
            compute_pi_baseline(j);
            clock_gettime(CLOCK_ID, &end);
            array_ci[i] = (double) ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC);
        }
        compute_ci(array_ci, TEST_COUNT);
        //printf("%lf ", (double) (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC);


        // OpenMP with 2 threads
        for(i = 0; i < loop; i++) {
            clock_gettime(CLOCK_ID, &start);
            compute_pi_openmp(j, 2);
            clock_gettime(CLOCK_ID, &end);
            array_ci[i] = (double) ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC);
        }
        compute_ci(array_ci, TEST_COUNT);
        //printf("%lf ", (double) (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC);

        // OpenMP with 4 threads
        for(i = 0; i < loop; i++) {
            clock_gettime(CLOCK_ID, &start);
            compute_pi_openmp(j, 4);
            clock_gettime(CLOCK_ID, &end);
            array_ci[i] = (double) ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC);
        }
        compute_ci(array_ci, TEST_COUNT);
        //printf("%lf ", (double) (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC);

#if defined(AVX)
        // AVX SIMD
        clock_gettime(CLOCK_ID, &start);
        for(i = 0; i < loop; i++) {
            compute_pi_avx(j);
        }
        clock_gettime(CLOCK_ID, &end);
        printf("%lf ", (double) (end.tv_sec - start.tv_sec) +
               (end.tv_nsec - start.tv_nsec)/ONE_SEC);
#endif

#if defined(AVXUNROLL)
        // AVX SIMD + Loop unrolling
        clock_gettime(CLOCK_ID, &start);
        for(i = 0; i < loop; i++) {
            compute_pi_avx_unroll(j);
        }
        clock_gettime(CLOCK_ID, &end);
        printf("%lf\n", (double) (end.tv_sec - start.tv_sec) +
               (end.tv_nsec - start.tv_nsec)/ONE_SEC);
#endif
        printf("\n");
    }
    return 0;
}
