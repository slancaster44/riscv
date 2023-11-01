
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include "environment.h"

static uint32_t* memory_base;

void Ecall(Hart* h) {}

void Ebreak() {
    exit(0);
}

void InitializeMemory(uint32_t* binary, size_t memory_size) {
    memory_base = malloc(memory_size);
    memcpy(memory_base, binary, memory_size);
}

void CleanupMemory() {
    if (memory_base != NULL) {
        free(memory_base);
    }
}

uint32_t* GetMemory() {
    return memory_base;
}
