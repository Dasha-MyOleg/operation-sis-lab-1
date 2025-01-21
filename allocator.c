#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>

// Maximum allowed allocation size (1 GB)
#define MAX_ALLOC_SIZE (10 * 1024 * 1024)  // 10 MB max allocation limit


typedef struct MemoryBlock {
    size_t size;
    int free;
    struct MemoryBlock *next;
} MemoryBlock;

MemoryBlock *head = NULL;

// Align size to system pointer size for optimal allocation
size_t align_size(size_t size) {
    return (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
}

// Allocate memory
void *mem_alloc(size_t size) {
    if (size == 0 || size > MAX_ALLOC_SIZE) {
        printf("Error: Allocation request of %zu bytes exceeds the allowed limit!\n", size);
        return NULL;  // Prevent allocation of 0 bytes or exceeding limit
    }

    size = align_size(size);
    MemoryBlock *current = head, *prev = NULL;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            current->free = 0;
            return (void*)(current + 1);
        }
        prev = current;
        current = current->next;
    }

    // Allocate new block
    size_t total_size = size + sizeof(MemoryBlock);
    MemoryBlock *new_block = (MemoryBlock*) VirtualAlloc(NULL, total_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!new_block) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    new_block->size = size;
    new_block->free = 0;
    new_block->next = NULL;

    if (prev) {
        prev->next = new_block;
    } else {
        head = new_block;
    }

    return (void*)(new_block + 1);
}


// Free allocated memory
void mem_free(void *ptr) {
    if (ptr == NULL) return;  // Freeing NULL should do nothing

    MemoryBlock *block = (MemoryBlock*)ptr - 1;
    if (block->free) {
        printf("Warning: Attempted double free detected!\n");
        return;
    }
    block->free = 1;
}

// Reallocate memory
void *mem_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return mem_alloc(size);
    }
    if (size == 0) {
        mem_free(ptr);
        return NULL;
    }

    MemoryBlock *block = (MemoryBlock*)ptr - 1;
    if (block->size >= size) {
        return ptr;  // No need to move, enough space
    }

    void *new_ptr = mem_alloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, block->size);
        mem_free(ptr);
    }
    return new_ptr;
}

// Show memory blocks for debugging
void mem_show() {
    MemoryBlock *current = head;
    printf("Memory blocks:\n");
    while (current) {
        printf("  Block at %p: size=%zu, free=%d\n", (void*)current, current->size, current->free);
        current = current->next;
    }
}
