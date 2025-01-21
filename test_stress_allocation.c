#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "allocator.h"

#define TEST_ITERATIONS 100

int main() {
    srand(time(NULL));
    void *blocks[TEST_ITERATIONS];

    printf("Starting memory allocator tests...\n");

    // ???? 1: ????????? ????????? ???'???
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        size_t size = (rand() % 1024) + 1;
        blocks[i] = mem_alloc(size);
        if (blocks[i]) {
            printf("Allocated block %d: %zu bytes\n", i, size);
        } else {
            printf("Allocation failed for block %d\n", i);
        }
    }

    mem_show();

    // ???? 2: ????????? ?????????? ??????
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        if (blocks[i] && (rand() % 2 == 0)) {
            mem_free(blocks[i]);
            printf("Freed block %d\n", i);
        }
    }

    mem_show();

    printf("Memory testing completed successfully.\n");

    return 0;
}
