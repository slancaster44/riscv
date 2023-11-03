#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "environment.h"
#include "encoding.h"
#include "decoding.h"

void IllegalInstruction(uint32_t i) {
    printf("Illegal Instruction: 0x%08X\n", i);
    exit(1);
}

inline void ExecLui(Hart* h, uint32_t ins) {
    uint8_t rd = Rd(ins);
    uint64_t imm = SignExtend64(U_Imm(ins));
    WriteRegister64(h, rd, imm);
}

inline void ExecAuipc(Hart* h, uint32_t ins) {

}

inline void ExecJal(Hart* h, uint32_t ins) {

}

inline void ExecJalr(Hart* h, uint32_t ins) {

}

inline void ExecBranch(Hart* h, uint32_t ins) {

}

inline void ExecLoad(Hart* h, uint32_t ins) {

}

inline void ExecStore(Hart* h, uint32_t ins) {

}

inline void ExecArithImm(Hart* h, uint32_t ins) {
    uint8_t funct3 = Funct3(ins);
    uint8_t rd = Rd(ins);
    uint32_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint32_t imm = I_Imm(ins);

    switch (funct3) {
    case ADDI_FUNCT3:
        WriteRegister64(h, rd, rs1_value + imm);
        break;
    default:
        IllegalInstruction(ins);
    }
}

inline void ExecArith32Imm(Hart* h, uint32_t ins) {

}

inline void ExecArithReg(Hart* h, uint32_t ins) {

}

inline void ExecArith32Reg(Hart* h, uint32_t ins) {

}

inline void ExecFence(Hart* h, uint32_t ins) {

}

inline void ExecEnvCsr(Hart* h, uint32_t ins) {

}

void ExecIns(Hart* h) {
    uint32_t* mem = GetMemory();
    size_t pc_idx = h->PC.Integer64 / sizeof(uint32_t);

    uint32_t instruction = mem[pc_idx];
    h->PC.Integer64 += sizeof(uint32_t);

    uint8_t op = Opcode(instruction);
    switch (op) {
    case LUI_OP:
        ExecLui(h, ins)
        break;
    case ARITH_IMM_OP:
        ExecArithImm(h, ins);
        break;
    default:
        IllegalInstruction(instruction);
    }
}

