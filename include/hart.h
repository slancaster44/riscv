#ifndef HART_H
#define HART_H

#include "data.h"

typedef struct {
    Register registers[31];
    Register PC;
} Hart;

uint32_t ReadRegister32(Hart* h, uint8_t register_number);
void  WriteRegister32(Hart* h, uint8_t register_number, uint32_t v);

#endif