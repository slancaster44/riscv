#ifndef MEM_H
#define MEM_H

#include "hart.h"

void Ecall(Hart* h);
void Ebreak();

void InitializeMemory(uint32_t* binary, size_t mem_size);
uint32_t* GetMemory();

#endif