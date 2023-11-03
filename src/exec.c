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

inline void ArithImmSRFunct7(Hart* h, uint32_t ins) {
    uint8_t funct3 = Funct3(ins);
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint64_t imm = SignExtend64(I_Imm(ins));
    uint8_t shift_amount;

    if (funct3 == SRAI_FUNCT3) { //srai
        uint8_t shift_amount = (uint8_t) IndexBits(I_Imm(ins), 0, 5);
        WriteRegister64(h, rd, ((int64_t) rs1_value) >> shift_amount); //in c, when shifting signed values, its arithmetic
    } else {
        IllegalInstruction(ins);
    }
}

inline void ArithImmStdFunct7(Hart* h, uint32_t ins) {
    uint8_t funct3 = Funct3(ins);
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint64_t imm = SignExtend64(I_Imm(ins));

    uint8_t shift_amount;
    uint8_t funct7;

    switch (funct3) {
    case ADDI_FUNCT3:
        WriteRegister64(h, rd, rs1_value + imm);
        break;
    case SLTIU_FUNCT3:
        WriteRegister64(h, rd, rs1_value < imm);
        break;
    case SLTI_FUNCT3:
        WriteRegister64(h, rd, (int64_t) rs1_value < (int64_t) imm);
        break;
    case XORI_FUNCT3:
        WriteRegister64(h, rd, rs1_value ^ imm);
        break;
    case ORI_FUNCT3:
        WriteRegister64(h, rd, rs1_value | imm);
        break;
    case ANDI_FUNCT3:
        WriteRegister64(h, rd, rs1_value & imm);
        break;
    case SLLI_FUNCT3:
        shift_amount = (uint8_t) IndexBits(I_Imm(ins), 0, 5);
        funct7 = Funct7(ins);
        if (funct7 != STD_FUNCT7) { IllegalInstruction(ins); }
        WriteRegister64(h, rd, rs1_value << shift_amount);
        break;
    case SRLI_FUNCT3:
        shift_amount = (uint8_t) IndexBits(I_Imm(ins), 0, 5);
        funct7 = Funct7(ins);
        if (funct7 != STD_FUNCT7) { IllegalInstruction(ins); }
        WriteRegister64(h, rd, rs1_value >> shift_amount);
        break;
    default:
        IllegalInstruction(ins);
        break;
    }
}

inline void ExecArithImm(Hart* h, uint32_t ins) {
    uint8_t funct7 = Funct7(ins);

    if (funct7 == SR_FUNCT7) {
        ArithImmSRFunct7(h, ins);
    } else {
        ArithImmStdFunct7(h, ins);
    }
}

inline void ExecArith32Imm(Hart* h, uint32_t ins) {

}

inline void ArithRegSRFunct7(Hart* h, uint32_t ins) {
    uint8_t funct3 = Funct3(ins);
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint64_t rs2_value = ReadRegister64(h, Rs2(ins));

    switch (funct3) {
    case SUB_FUNCT3:
        WriteRegister64(h, rd, rs1_value - rs2_value);
        break;
    case SRA_FUNCT3:
        WriteRegister64(h, rd, (int64_t) rs1_value >> (0b111111 & rs2_value));
        break;
    default:
        IllegalInstruction(ins);
    }
}

inline void ArithRegSTDFunct7(Hart* h, uint32_t ins) {
    uint8_t funct3 = Funct3(ins);
    uint8_t rd = Rd(ins);
    uint64_t rs1_value = ReadRegister64(h, Rs1(ins));
    uint64_t rs2_value = ReadRegister64(h, Rs2(ins));

    switch (funct3) {
    case ADD_FUNCT3:
        WriteRegister64(h, rd, rs1_value + rs2_value);
        break;
    case SLL_FUNCT3:
        WriteRegister64(h, rd, rs1_value << rs2_value);
        break;
    case SLT_FUNCT3:
        WriteRegister64(h, rd, (int64_t) rs1_value < (int64_t) rs2_value);
        break;
    case SLTU_FUNCT3:
        WriteRegister64(h, rd, rs1_value < rs2_value);
        break;
    case XOR_FUNCT3:
        WriteRegister64(h, rd, rs1_value ^ rs2_value);
        break;
    case OR_FUNCT3:
        WriteRegister64(h, rd, rs1_value | rs2_value);
        break;
    case AND_FUNCT3:
        WriteRegister64(h, rd, rs1_value & rs2_value);
        break;
    default:
        IllegalInstruction(ins);
    }
}

inline void ExecArithReg(Hart* h, uint32_t ins) {
    uint8_t funct7 = Funct7(ins);

    if (funct7 == SR_FUNCT7) {
        ArithRegSRFunct7(h, ins);
    } else if (funct7 == STD_FUNCT7) {
        ArithRegSTDFunct7(h, ins);
    } else {
        IllegalInstruction(ins);
    }
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
        ExecLui(h, instruction);
        break;
    case ARITH_IMM_OP:
        ExecArithImm(h, instruction);
        break;
    case ARITH_REG_OP:
        ExecArithReg(h, instruction);
        break;
    default:
        IllegalInstruction(instruction);
    }
}

