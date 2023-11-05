#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "environment.h"
#include "encoding.h"
#include "decoding.h"

static DecodingNode* lookupTree;

void Lui(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t imm = SignExtend64(U_Imm(ins));
    WriteRegister64(h, rd, imm);
}

#define IMM_ARITH_64(HART, INS, OP) {                       \
    uint8_t rd = Rd(INS);                                   \
    uint64_t rs1_value = ReadRegister64(HART, Rs1(INS));    \
    uint64_t imm = SignExtend64(I_Imm(INS));                \
    WriteRegister64(HART, rd, rs1_value OP imm);            \
}

#define REG_ARITH_64(HART, INS, OP) {                       \
    uint8_t rd = Rd(INS);                                   \
    uint64_t rs1_value = ReadRegister64(HART, Rs1(INS));    \
    uint64_t rs2_value = ReadRegister64(HART, Rs2(INS));    \
    WriteRegister64(HART, rd, rs1_value OP rs2_value);      \
}

#define REG_ARITH_64_6BIT(HART, INS, OP) {                          \
    uint8_t rd = Rd(INS);                                           \
    uint64_t rs1_value = ReadRegister64(HART, Rs1(INS));            \
    uint64_t rs2_value = ReadRegister64(HART, Rs2(INS));            \
    WriteRegister64(HART, rd, rs1_value OP (0b111111 & rs2_value)); \
}

void Addi64(Hart* h, uint32_t ins) {
    IMM_ARITH_64(h, ins, +);
}

void Sltiu64(Hart* h, uint32_t ins) {
    IMM_ARITH_64(h, ins, <);
}

void Xori64(Hart* h, uint32_t ins) {
    IMM_ARITH_64(h, ins, ^);
}

void Ori64(Hart* h, uint32_t ins) {
    IMM_ARITH_64(h, ins, |);
}

void Andi64(Hart* h, uint32_t ins) {
    IMM_ARITH_64(h, ins, &);
}

void Srai64(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint8_t shift_amount = (uint8_t) I_Imm(ins) & 0b111111;
    WriteRegister64(h, rd, ((int64_t) rs1_value) >> shift_amount); //in c, when shifting signed values, its arithmetic
}

void Srli64(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint8_t shift_amount = (uint8_t) I_Imm(ins) & 0b111111;
    WriteRegister64(h, rd, rs1_value >> shift_amount);
}

void Slli64(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint8_t shift_amount = (uint8_t) I_Imm(ins) & 0b111111;
    WriteRegister64(h, rd, rs1_value << shift_amount);
}

void Slti64(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint64_t imm = SignExtend64(I_Imm(ins));
    WriteRegister64(h, rd, (int64_t) rs1_value < (int64_t) imm);
}

void Add64(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, +);
}

void Sub64(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, -);
}

void Sll64(Hart* h, uint32_t ins) {
    REG_ARITH_64_6BIT(h, ins, <<); 
}

void Sltu64(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, <);
}

void Xor64(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, ^);
}

void Or64(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, |);
}

void And64(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, &);
}

void Srl64(Hart* h, uint32_t ins) {
    REG_ARITH_64_6BIT(h, ins, >>); 
}

void Sra64(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint64_t rs2_value = ReadRegister64(h, Rs2(ins));
    WriteRegister64(h, rd, (int64_t) rs1_value >> (0b111111 & rs2_value));
}


void Slt64(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint64_t rs2_value = ReadRegister64(h, Rs2(ins));
    WriteRegister64(h, rd, (int64_t) rs1_value < (int64_t) rs2_value);
}

DecodingNode* NewTree() {
    DecodingNode* root = NewNode();

    AddInstruction(root, "xxxxxxxxxxxxxxxxxxxxxxxxx0110111", Lui);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx000xxxxx0010011", Addi64);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx011xxxxx0010011", Sltiu64);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx010xxxxx0010011", Slti64);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx100xxxxx0010011", Xori64);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx110xxxxx0010011", Ori64);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx111xxxxx0010011", Andi64);
    AddInstruction(root, "000000xxxxxxxxxxx001xxxxx0010011", Slli64);
    AddInstruction(root, "000000xxxxxxxxxxx101xxxxx0010011", Srli64);
    AddInstruction(root, "010000xxxxxxxxxxx101xxxxx0010011", Srai64);

    AddInstruction(root, "0000000xxxxxxxxxx000xxxxx0110011", Add64);
    AddInstruction(root, "0100000xxxxxxxxxx000xxxxx0110011", Sub64);
    AddInstruction(root, "0000000xxxxxxxxxx001xxxxx0110011", Sll64);
    AddInstruction(root, "0000000xxxxxxxxxx010xxxxx0110011", Slt64);
    AddInstruction(root, "0000000xxxxxxxxxx011xxxxx0110011", Sltu64);
    AddInstruction(root, "0000000xxxxxxxxxx100xxxxx0110011", Xor64);
    AddInstruction(root, "0000000xxxxxxxxxx101xxxxx0110011", Srl64);
    AddInstruction(root, "0100000xxxxxxxxxx101xxxxx0110011", Sra64);
    AddInstruction(root, "0000000xxxxxxxxxx110xxxxx0110011", Or64);
    AddInstruction(root, "0000000xxxxxxxxxx111xxxxx0110011", And64);
    
    return root;
}

void FreeTree() {
    if (lookupTree != NULL) {
        FreeNode(lookupTree);
    }
}

void ExecIns(Hart* h) {
    uint32_t* mem = GetMemory();
    size_t pc_idx = h->PC.Integer64 / sizeof(uint32_t);

    uint32_t instruction = mem[pc_idx];
    h->PC.Integer64 += sizeof(uint32_t);

    if (lookupTree == NULL) {
        lookupTree = NewTree();
        atexit(FreeTree);
    }

    InstructionFunction insfn = Lookup(lookupTree, instruction);
    insfn(h, instruction);
}

