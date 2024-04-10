#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./program_name <num_threads>\n");
        return 1;
    }

    int num_threads = atoi(argv[1]);

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        printf("Hello World from Thread %d!\n", thread_id);
    }

    return 0;
}
