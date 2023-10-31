#ifndef HART_H
#define HART_H

#include <stdint.h>

typedef union {
    uint32_t Integer32;
    uint64_t Integer64;
} Register;

typedef struct {
    Register registers[32];
    Register PC;
} Hart;

uint32_t ReadRegister32(Hart* h, uint8_t register_number);
void  WriteRegister32(Hart* h, uint8_t register_number, uint32_t v);

uint64_t ReadRegister64(Hart* h, uint8_t register_number);
void  WriteRegister64(Hart* h, uint8_t register_number, uint64_t v);

#endif