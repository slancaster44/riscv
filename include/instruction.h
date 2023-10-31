#ifndef INS_H
#define INS_H

#include <stdint.h>

uint32_t MaskBits(uint32_t i, uint8_t start_bit, uint8_t end_bit);
uint32_t IndexBits(uint32_t i, uint8_t start_bit, uint8_t end_bit);

uint32_t I_Imm(uint32_t i);
uint32_t S_Imm(uint32_t i);
uint32_t B_Imm(uint32_t i);
uint32_t U_Imm(uint32_t i);
uint32_t J_Imm(uint32_t i);

uint8_t Opcode(uint32_t i);

uint8_t Funct3(uint32_t i);
uint8_t Funct7(uint32_t i);

uint8_t Rd(uint32_t i);
uint8_t Rs1(uint32_t i);
uint8_t Rs2(uint32_t i);

#endif