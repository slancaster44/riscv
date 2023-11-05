#ifndef CODES_H
#define CODES_H

#define LUI_OP 0b0110111
#define AUIPC_OP 0b0010111
#define JAL_OP 0b1101111

#define JALR_OP 0b1100111
#define JALR_FUNCT3 0b000

#define BRANCH_OP 0b1100011
#define BEQ_FUNCT3 0b000
#define BNE_FUNCT3 0b001
#define BLT_FUNCT3 0b100
#define BGE_FUNCT3 0b101
#define BLTU_FUNCT3 0b110
#define BGEU_FUNCT3 0b111

#define LOAD_OP 0b0000011
#define LB_FUNCT3 0b000
#define LH_FUNCT3 0b001
#define LW_FUNCT3 0b010
#define LBU_FUNCT3 0b100
#define LHU_FUNCT3 0b101
#define LWU_FUNCT3 0b110
#define LD_FUNCT3 0b011

#define STORE_OP 0b0100011
#define SB_FUNCT3 0b000
#define SH_FUNCT3 0b001
#define SW_FUNCT3 0b010
#define SD_FUNCT3 0b011

#define STD_FUNCT7 0b0000000
#define SR_FUNCT7 0b0100000

#define ARITH_IMM_OP 0b0010011
#define ADDI_FUNCT3 0b000   // 64
#define SLTI_FUNCT3 0b010   // 64
#define SLTIU_FUNCT3 0b011  // 64
#define XORI_FUNCT3 0b100   // 64
#define ORI_FUNCT3 0b110    // 64
#define ANDI_FUNCT3 0b111   // 64
#define SLLI_FUNCT3 0b001   // 64
#define SRLI_FUNCT3 0b101   // 64
#define SRAI_FUNCT3 0b101   // 64

#define ARITH32_IMM_OP 0b0011011
#define ARITH32_REG_OP 0b0111011

#define ARITH_REG_OP 0b0110011 
#define ADD_FUNCT3 0b000    //64
#define SUB_FUNCT3 0b000    //64
#define SLL_FUNCT3 0b001    //64
#define SLT_FUNCT3 0b010    //64
#define SLTU_FUNCT3 0b011   //64
#define XOR_FUNCT3 0b100    //64
#define SRL_FUNCT3 0b101    //64
#define SRA_FUNCT3 0b101    //64
#define OR_FUNCT3 0b110     //64
#define AND_FUNCT3 0b111    //64

#define FENCE_OP 0b0001111
#define FENCE_FUNCT3 0b000
#define FENCEI_FUNCT3 0b001

#define ENV_CSR_OP 0b1110011
#define ECALL_FUNCT3 0b000
#define EBREAK_FUNCT4 0b000
#define ECALL_I_IMM 0b000000000000
#define EBREAK_I_IMM 0b000000000001

/* TODO: Implement the CSRs
#define CSRRW_FUNCT3 0b001
#define CSRRS_FUNCT3 0b010
#define CSRRC_FUNCT3 0b011
#define CSRRWI_FUNCT3 0b101
#define CSRRSI_FUNCT3 0b110
#define CSRRCI_FUNCT3 0b111
*/

#endif
