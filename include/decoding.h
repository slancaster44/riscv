#ifndef INS_H
#define INS_H

#include <stdint.h>
#include "hart.h"

typedef void(*InstructionFunction)(Hart*, uint32_t);

typedef struct {
    uint8_t bit;
    struct DecodingNode* Left; //0
    struct DecodingNode* Right; //1
    struct DecodingNode* Neutral; //it doesn't matter
    InstructionFunction insFn;
} DecodingNode;

DecodingNode* NewNode();
void FreeNode(DecodingNode* n);
InstructionFunction Lookup(DecodingNode* dn, uint32_t ins);
void AddInstruction(DecodingNode* root, const char* pattern, InstructionFunction insfn);

uint32_t MaskBits(uint32_t i, uint8_t start_bit, uint8_t end_bit);
uint32_t IndexBits(uint32_t i, uint8_t start_bit, uint8_t end_bit);
uint8_t IndexBit(uint32_t i, uint8_t bit);
uint32_t SignExtend(uint32_t base, uint32_t sign, uint8_t start);
uint64_t SignExtend64(uint32_t base);

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