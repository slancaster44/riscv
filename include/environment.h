#ifndef ENV_H
#define ENV_H

#include <stddef.h>

#include "hart.h"

void Ecall(Hart* h);
void Ebreak();

void InitializeMemory(uint32_t* binary, size_t mem_size);
void CleanupMemory();
uint32_t* GetMemory();

#endif