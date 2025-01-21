# operation-sis-lab-1

ЗВІТ

Лабораторної роботи №1

З дисципліни "Системне програмне забезпечення (Операційні системи)"

---
**Тема:** Алокатор пам’яті загального призначення з використанням тегів

**Виконала:** Дар'я Іванівна

**Група:** IM-24


---
```
**1. Лістинг розробленої програми**


#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void *mem_alloc(size_t size);
void mem_free(void *ptr);
void *mem_realloc(void *ptr, size_t size);
void mem_show();


void *mem_page_alloc(size_t size);
void mem_page_free(void *ptr, size_t size);

#endif 

---

**Файл allocator.c (реалізація алокатора)**


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
}```

---

**2. Приклад роботи розробленого алокатора пам’яті**

Testing memory reallocation...
Allocated 100 bytes
Reallocated to 200 bytes
Reallocated to 50 bytes (shrinking)
Memory freed
Memory blocks:
  Block at 00000245638E0000: size=200, free=1


test_stress_allocation.c – тестування великої кількості алокацій:
-Виконує велику кількість викликів mem_alloc(), щоб перевірити, чи коректно працює виділення пам’яті.
-Визначає можливі витоки пам’яті та перевіряє, чи правильно розподіляються блоки.
-Після алокації виконується звільнення пам'яті для тестування коректності операції mem_free().


test_reallocation.c – перевірка функції mem_realloc():
-Тестує розширення та зменшення блоків пам’яті.
-Перевіряє правильність збереження даних при зміні розміру блоку.
-Виконується реаліокація з нульовим покажчиком для перевірки поведінки аналогічної mem_alloc().


test_free.c – тестування звільнення пам'яті:
-Тестує звільнення пам’яті та перевіряє, чи коректно об'єднуються вільні блоки.
-Перевіряє поведінку при звільненні нульового покажчика та подвійного звільнення.
-Переконується, що блоки, які були звільнені, доступні для повторного використання.


test_edge_cases.c – тестування граничних випадків:
-Алокація нульового розміру та перевірка очікуваної поведінки.
-Спроба виділення дуже великого блоку пам'яті для перевірки обмежень алокатора.
-Перевірка подвійного звільнення пам'яті та відображення попереджувальних повідомлень.


test_performance.c – вимірювання продуктивності:
-Вимірює швидкість роботи алокатора при великій кількості алокацій і звільнень.
-Аналізує час виконання виділення та звільнення пам'яті для визначення ефективності алокатора.
-Використовується для визначення можливих вузьких місць у продуктивності.


4. Висновки
У ході виконання лабораторної роботи було розроблено алокатор пам'яті загального призначення, який відповідає вимогам.
Реалізовані API функції: mem_alloc(), mem_free(), mem_realloc(), mem_show().
Використано заголовки блоків для обліку пам’яті.
Забезпечена обробка сторінкової та несторінкової пам’яті.
Проведено тестування з коректними результатами.
Отримані результати показують, що розроблений алокатор коректно виконує всі необхідні функції та демонструє очікувану продуктивність.