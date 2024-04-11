#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double serial_sum(double *x, size_t size) {
    double sum_val = 0.0;
    for (size_t i = 0; i < size; i++) {
        sum_val += x[i];
    }
    return sum_val;
}
//question 2
double omp_sum(double *x, size_t size) {
    double sum_val = 0.0;
    #pragma omp parallel for reduction(+:sum_val)
    for (size_t i = 0; i < size; i++) {
        sum_val += x[i];
    }
    return sum_val;
}

//question 3
double omp_critical_sum(double *x, size_t size) {
    double sum_val = 0.0;
    #pragma omp parallel
    {
        double local_sum = 0.0;
        #pragma omp for
        for (size_t i = 0; i < size; i++) {
            local_sum += x[i];
        }
        #pragma omp critical
        {
            sum_val += local_sum;
        }
    }
    return sum_val;
}

void generate_random(double *input, size_t size) {
    for (size_t i = 0; i < size; i++) {
        input[i] = rand() / (double)(RAND_MAX);
    }
}

#define MAX_THREADS 128

//question 4
double omp_local_sum(double *x, size_t size, int num_threads) {
    double local_sum[MAX_THREADS] = {0.0};
    double final_sum = 0.0;
    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        double local = 0.0;
        #pragma omp for
        for (size_t i = 0; i < size; i++) {
            local += x[i];
        }
        local_sum[thread_id] = local;
        #pragma omp barrier
        #pragma omp master
        {
            for (int i = 0; i < num_threads; i++) {
                final_sum += local_sum[i];
            }
        }
    }
    return final_sum;
}

//question 5
double opt_local_sum(double *x, size_t size, int num_threads) {
    double local_sum[MAX_THREADS] = {0.0};
    double final_sum = 0.0;
    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        double local = 0.0;
        #pragma omp for
        for (size_t i = 0; i < size; i++) {
            local += x[i];
        }
        local_sum[thread_id] = local;
        #pragma omp barrier

        // Padding to avoid false sharing
        #pragma omp single
        {
            for (int i = 0; i < num_threads; i++) {
                final_sum += local_sum[i];
            }
        }
    }
    return final_sum;
}


int main() {
    const size_t size = 10000000; // at least 10^7 elements
    double *data = (double *)malloc(size * sizeof(double));

    // Generate random data
    generate_random(data, size);

    // Serial Code Performance Measurement
    double serial_time_avg = 0.0;
    double serial_time_stddev = 0.0;
    for (int i = 0; i < 10; i++) {
        double start = omp_get_wtime();
        double result = serial_sum(data, size);
        double end = omp_get_wtime();
        double elapsed = end - start;
        serial_time_avg += elapsed;
        serial_time_stddev += elapsed * elapsed;
    }
    serial_time_avg /= 10.0;
    serial_time_stddev = sqrt((serial_time_stddev / 10.0) - (serial_time_avg * serial_time_avg));

    printf("Serial code execution time (average): %.6f seconds\n", serial_time_avg);
    printf("Serial code execution time (standard deviation): %.6f seconds\n", serial_time_stddev);

    const int SIZE = 9;
    int num_treads_array[] = { 1, 2, 4, 8, 16, 20, 24, 28, 32 };

    // Parallel Version (omp_sum) Performance Measurement
    printf("\nOpenMP Parallel Version (omp_sum):\n");
    for (int i = 0; i < SIZE; i++) {
        int num_threads = num_treads_array[i];
        omp_set_num_threads(num_threads);
        double parallel_time_avg = 0.0;
        double parallel_time_stddev = 0.0;
        for (int i = 0; i < 10; i++) {
            double start = omp_get_wtime();
            double result = omp_sum(data, size);
            double end = omp_get_wtime();
            double elapsed = end - start;
            parallel_time_avg += elapsed;
            parallel_time_stddev += elapsed * elapsed;
        }
        parallel_time_avg /= 10.0;
        parallel_time_stddev = sqrt((parallel_time_stddev / 10.0) - (parallel_time_avg * parallel_time_avg));

        printf("Threads: %d, Execution time (average): %.6f seconds, Execution time (standard deviation): %.6f seconds\n",
            num_threads, parallel_time_avg, parallel_time_stddev);
    }

    // Critical Section (omp_critical_sum) Performance Measurement
    printf("\nOpenMP Parallel Version with Critical Section (omp_critical_sum):\n");
    for (int i = 0; i < SIZE; i++) {
        int num_threads = num_treads_array[i];
        omp_set_num_threads(num_threads);
        double parallel_time_avg = 0.0;
        double parallel_time_stddev = 0.0;
        for (int i = 0; i < 10; i++) {
            double start = omp_get_wtime();
            double result = omp_critical_sum(data, size);
            double end = omp_get_wtime();
            double elapsed = end - start;
            parallel_time_avg += elapsed;
            parallel_time_stddev += elapsed * elapsed;
        }
        parallel_time_avg /= 10.0;
        parallel_time_stddev = sqrt((parallel_time_stddev / 10.0) - (parallel_time_avg * parallel_time_avg));

        printf("Threads: %d, Execution time (average): %.6f seconds, Execution time (standard deviation): %.6f seconds\n",
            num_threads, parallel_time_avg, parallel_time_stddev);
    }

    const int SIZE_2 = 4;
    int num_treads_array_2[] = { 1, 32, 64, 128 };

    // Performance Measurement for omp_local_sum
    printf("\nOpenMP Parallel Version with Local Sum (omp_local_sum):\n");
    for (int i = 0; i < SIZE_2; i++) {
        int num_threads = num_treads_array_2[i];
        double parallel_time_ls = 0.0;
        double parallel_time_ls_stddev = 0.0;
        for (int i = 0; i < 10; i++) {
            double start = omp_get_wtime();
            double result = omp_local_sum(data, size, num_threads);
            double end = omp_get_wtime();
            double elapsed = end - start;
            parallel_time_ls += elapsed;
            parallel_time_ls_stddev += elapsed * elapsed;
        }
        parallel_time_ls /= 10.0;
        parallel_time_ls_stddev = sqrt((parallel_time_ls_stddev / 10.0) - (parallel_time_ls * parallel_time_ls));
        printf("Threads: %d, Execution time (average): %.6f seconds, Execution time (standard deviation): %.6f seconds\n",
            num_threads, parallel_time_ls, parallel_time_ls_stddev);
    }

    // Performance Measurement for opt_local_sum
    printf("\nOptimized OpenMP Parallel Version with Local Sum (opt_local_sum):\n");
    for (int i = 0; i < SIZE_2; i++) {
        int num_threads = num_treads_array_2[i];
        double parallel_time_ls_optimized = 0.0;
        double parallel_time_ls_optimized_stddev = 0.0;
        for (int i = 0; i < 10; i++) {
            double start = omp_get_wtime();
            double result = opt_local_sum(data, size, num_threads);
            double end = omp_get_wtime();
            double elapsed = end - start;
            parallel_time_ls_optimized += elapsed;
            parallel_time_ls_optimized_stddev += elapsed * elapsed;
        }
        parallel_time_ls_optimized /= 10.0;
        parallel_time_ls_optimized_stddev = sqrt((parallel_time_ls_optimized_stddev / 10.0) - (parallel_time_ls_optimized * parallel_time_ls_optimized));
        printf("Threads: %d, Execution time (average): %.6f seconds, Execution time (standard deviation): %.6f seconds\n",
            num_threads, parallel_time_ls_optimized, parallel_time_ls_optimized_stddev);
    }

    free(data);
    return 0;
}
