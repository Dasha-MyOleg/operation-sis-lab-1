#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "allocator.h"

void test_reallocation() {
    printf("Testing memory reallocation...\n");

    // ????????? ????????? ???'???
    char *block = (char *)mem_alloc(100);
    if (!block) {
        printf("Initial allocation failed.\n");
        return;
    }
    strcpy(block, "InitialData");
    printf("Allocated 100 bytes\n");

    // ?????????? ??????? ?????
    block = (char *)mem_realloc(block, 200);
    if (!block) {
        printf("Reallocation to 200 bytes failed.\n");
        return;
    }
    printf("Reallocated to 200 bytes\n");

    // ????????? ?????????? ?????
    if (strcmp(block, "InitialData") == 0) {
        printf("Data integrity maintained after expansion.\n");
    } else {
        printf("Data corruption detected after expansion!\n");
    }

    // ????????? ??????? ?????
    block = (char *)mem_realloc(block, 50);
    if (!block) {
        printf("Reallocation to 50 bytes failed.\n");
        return;
    }
    printf("Reallocated to 50 bytes (shrinking)\n");

    if (strcmp(block, "InitialData") == 0) {
        printf("Data integrity maintained after shrinking.\n");
    } else {
        printf("Data corruption detected after shrinking!\n");
    }

    // ????????? ????????? ????? realloc(NULL)
    char *new_block = (char *)mem_realloc(NULL, 150);
    if (!new_block) {
        printf("Reallocation from NULL failed.\n");
    } else {
        printf("Reallocation from NULL successful, allocated 150 bytes.\n");
        mem_free(new_block);
    }

    // ?????????? ???'???
    mem_free(block);
    printf("Memory freed\n");

    mem_show();
}

int main() {
    test_reallocation();
    return 0;
}
