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

void Auipc(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t imm = SignExtend64(U_Imm(ins)); 
    WriteRegister64(h, rd, imm + h->PC.Integer64);
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

void Addi(Hart* h, uint32_t ins) {
    IMM_ARITH_64(h, ins, +);
}

void Sltiu(Hart* h, uint32_t ins) {
    IMM_ARITH_64(h, ins, <);
}

void Xori(Hart* h, uint32_t ins) {
    IMM_ARITH_64(h, ins, ^);
}

void Ori(Hart* h, uint32_t ins) {
    IMM_ARITH_64(h, ins, |);
}

void Andi(Hart* h, uint32_t ins) {
    IMM_ARITH_64(h, ins, &);
}

void Srai(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint8_t shift_amount = (uint8_t) I_Imm(ins) & 0b111111;
    WriteRegister64(h, rd, ((int64_t) rs1_value) >> shift_amount); //in c, when shifting signed values, its arithmetic
}

void Srli(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint8_t shift_amount = (uint8_t) I_Imm(ins) & 0b111111;
    WriteRegister64(h, rd, rs1_value >> shift_amount);
}

void Slli(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint8_t shift_amount = (uint8_t) I_Imm(ins) & 0b111111;
    WriteRegister64(h, rd, rs1_value << shift_amount);
}

void Slti(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint64_t imm = SignExtend64(I_Imm(ins));
    WriteRegister64(h, rd, (int64_t) rs1_value < (int64_t) imm);
}

void Add(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, +);
}

void Sub(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, -);
}

void Sll(Hart* h, uint32_t ins) {
    REG_ARITH_64_6BIT(h, ins, <<); 
}

void Sltu(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, <);
}

void Xor(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, ^);
}

void Or(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, |);
}

void And(Hart* h, uint32_t ins) {
    REG_ARITH_64(h, ins, &);
}

void Srl(Hart* h, uint32_t ins) {
    REG_ARITH_64_6BIT(h, ins, >>); 
}

void Sra(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint64_t rs2_value = ReadRegister64(h, Rs2(ins));
    WriteRegister64(h, rd, (int64_t) rs1_value >> (0b111111 & rs2_value));
}

void Slt(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint64_t rs2_value = ReadRegister64(h, Rs2(ins));
    WriteRegister64(h, rd, (int64_t) rs1_value < (int64_t) rs2_value);
}

void Addiw(Hart* h, uint32_t ins) {
    uint32_t rs1_value = ReadRegister32(h, Rs1(ins));
    uint32_t imm = I_Imm(ins);
    uint8_t rd = Rd(ins);
    WriteRegister64(h, rd, SignExtend64(rs1_value + imm));
}

void Slliw(Hart* h, uint32_t ins) {
    uint32_t rs1_value = ReadRegister32(h, Rs1(ins));
    uint32_t imm = I_Imm(ins);
    uint8_t rd = Rd(ins);
    WriteRegister64(h, rd, SignExtend64(rs1_value << (imm & 0b11111)));
}

void Srliw(Hart* h, uint32_t ins) {
    uint32_t rs1_value = ReadRegister32(h, Rs1(ins));
    uint32_t imm = I_Imm(ins);
    uint8_t rd = Rd(ins);
    WriteRegister64(h, rd, SignExtend64(rs1_value >> (imm & 0b11111)));
}

void Sraiw(Hart* h, uint32_t ins) {
    int32_t rs1_value = (int32_t) ReadRegister32(h, Rs1(ins));
    uint32_t imm = I_Imm(ins);
    uint8_t rd = Rd(ins);
    WriteRegister64(h, rd, SignExtend64(rs1_value >> (imm & 0b11111)));
}

void Addw(Hart* h, uint32_t ins) {
    uint32_t rs1_value = ReadRegister32(h, Rs1(ins));
    uint32_t rs2_value = ReadRegister32(h, Rs1(ins));
    uint8_t rd = ReadRegister32(h, Rd(ins));
    WriteRegister64(h, rd, SignExtend64(rs1_value + rs2_value));
}

void Subw(Hart* h, uint32_t ins) {
    uint32_t rs1_value = ReadRegister32(h, Rs1(ins));
    uint32_t rs2_value = ReadRegister32(h, Rs1(ins));
    uint8_t rd = ReadRegister32(h, Rd(ins));
    WriteRegister64(h, rd, SignExtend64(rs1_value - rs2_value));
}

void Sllw(Hart* h, uint32_t ins) {
    uint32_t rs1_value = ReadRegister32(h, Rs1(ins));
    uint32_t rs2_value = ReadRegister32(h, Rs1(ins)) & 0b11111;
    uint8_t rd = ReadRegister32(h, Rd(ins));
    WriteRegister64(h, rd, SignExtend64(rs1_value << rs2_value));
}

void Srlw(Hart* h, uint32_t ins) {
    uint32_t rs1_value = ReadRegister32(h, Rs1(ins));
    uint32_t rs2_value = ReadRegister32(h, Rs1(ins)) & 0b11111;
    uint8_t rd = ReadRegister32(h, Rd(ins));
    WriteRegister64(h, rd, SignExtend64(rs1_value >> rs2_value));
}

void Sraw(Hart* h, uint32_t ins) {
    int32_t rs1_value = (int32_t) ReadRegister32(h, Rs1(ins));
    uint32_t rs2_value = ReadRegister32(h, Rs1(ins)) & 0b11111;
    uint8_t rd = ReadRegister32(h, Rd(ins));
    WriteRegister64(h, rd, SignExtend64(rs1_value >> rs2_value));
}

DecodingNode* NewTree() {
    DecodingNode* root = NewNode();

    AddInstruction(root, "xxxxxxxxxxxxxxxxxxxxxxxxx0110111", Lui);
    AddInstruction(root, "xxxxxxxxxxxxxxxxxxxxxxxxx0010111", Auipc);

    AddInstruction(root, "xxxxxxxxxxxxxxxxx000xxxxx0010011", Addi);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx011xxxxx0010011", Sltiu);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx010xxxxx0010011", Slti);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx100xxxxx0010011", Xori);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx110xxxxx0010011", Ori);
    AddInstruction(root, "xxxxxxxxxxxxxxxxx111xxxxx0010011", Andi);
    AddInstruction(root, "000000xxxxxxxxxxx001xxxxx0010011", Slli);
    AddInstruction(root, "000000xxxxxxxxxxx101xxxxx0010011", Srli);
    AddInstruction(root, "010000xxxxxxxxxxx101xxxxx0010011", Srai);

    AddInstruction(root, "0000000xxxxxxxxxx000xxxxx0110011", Add);
    AddInstruction(root, "0100000xxxxxxxxxx000xxxxx0110011", Sub);
    AddInstruction(root, "0000000xxxxxxxxxx001xxxxx0110011", Sll);
    AddInstruction(root, "0000000xxxxxxxxxx010xxxxx0110011", Slt);
    AddInstruction(root, "0000000xxxxxxxxxx011xxxxx0110011", Sltu);
    AddInstruction(root, "0000000xxxxxxxxxx100xxxxx0110011", Xor);
    AddInstruction(root, "0000000xxxxxxxxxx101xxxxx0110011", Srl);
    AddInstruction(root, "0100000xxxxxxxxxx101xxxxx0110011", Sra);
    AddInstruction(root, "0000000xxxxxxxxxx110xxxxx0110011", Or);
    AddInstruction(root, "0000000xxxxxxxxxx111xxxxx0110011", And);

    AddInstruction(root, "xxxxxxxxxxxxxxxxx000xxxxx0011011", Addiw);
    AddInstruction(root, "0000000xxxxxxxxxx001xxxxx0011011", Slliw);
    AddInstruction(root, "0000000xxxxxxxxxx101xxxxx0011011", Srliw);
    AddInstruction(root, "0100000xxxxxxxxxx101xxxxx0011011", Sraiw);

    AddInstruction(root, "0000000xxxxxxxxxx000xxxxx0111011", Addw);
    AddInstruction(root, "0100000xxxxxxxxxx000xxxxx0111011", Subw);
    AddInstruction(root, "0000000xxxxxxxxxx001xxxxx0111011", Sllw);
    AddInstruction(root, "0000000xxxxxxxxxx101xxxxx0111011", Srlw);
    AddInstruction(root, "0100000xxxxxxxxxx101xxxxx0111011", Sraw);
    
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

    if (lookupTree == NULL) {
        lookupTree = NewTree();
        atexit(FreeTree);
    }

    InstructionFunction insfn = Lookup(lookupTree, instruction);
    insfn(h, instruction);

    h->PC.Integer64 += sizeof(uint32_t);
}

