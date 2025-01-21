#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "allocator.h"

#define NUM_ALLOCS 100000
#define MAX_ALLOC_SIZE 1024

int main() {
    printf("Starting performance test...\n");
    fflush(stdout);

    void *pointers[NUM_ALLOCS];
    clock_t start_time, end_time;
    double elapsed_time;

    srand((unsigned int)time(NULL));

    start_time = clock();
    for (int i = 0; i < NUM_ALLOCS; i++) {
        size_t size = rand() % MAX_ALLOC_SIZE + 1;
        pointers[i] = mem_alloc(size);
        if (!pointers[i]) {
            printf("Allocation failed at iteration %d\n", i);
            break;
        }
        if (i % 10000 == 0) {
            printf("Allocated %d blocks...\n", i);
            fflush(stdout);
        }
    }
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Allocation phase completed in %.2f seconds.\n", elapsed_time);

    start_time = clock();
    for (int i = 0; i < NUM_ALLOCS; i++) {
        mem_free(pointers[i]);
        if (i % 5000 == 0) {
            printf("Freed %d blocks...\n", i);
            fflush(stdout);
        }
    }
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Deallocation phase completed in %.2f seconds.\n", elapsed_time);

    printf("Performance test completed successfully.\n");

    return 0;
}

