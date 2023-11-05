#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "decoding.h"

/* All the stuff in this file will make since if you read the
 * RV32I section in the risc-v manual
 */

//Index froms start_bit to end_bit including start_bit but not end_bit
uint32_t MaskBits(uint32_t i, uint8_t start_bit, uint8_t end_bit) {
    uint32_t complete_mask = 0xFFFFFFFF;
    uint32_t left_mask = complete_mask >> (31 - end_bit);
    uint32_t final_mask = left_mask & (complete_mask << start_bit);
    return i & final_mask;
}

uint32_t IndexBits(uint32_t i, uint8_t start_bit, uint8_t end_bit) {
    return MaskBits(i, start_bit, end_bit) >> start_bit;
}

uint8_t IndexBit(uint32_t i, uint8_t bit) {
    uint32_t mask = 1 << bit;
    return (i & mask) >> bit;
}

uint32_t SignExtend(uint32_t base, uint32_t sign, uint8_t start) {
    uint32_t sign_mask = sign ? 0xFFFFFFFF : 0x0;
    sign_mask = MaskBits(sign_mask, start, 31);
    return base | sign_mask;
}

uint64_t SignExtend64(uint32_t base) {
    uint64_t sign_mask = IndexBits(base, 31, 31) ? 0xFFFFFFFF : 0x0;
    return base | (sign_mask << 32);
}

uint32_t I_Imm(uint32_t i) {
    uint32_t imm = IndexBits(i, 20, 30);
    uint32_t sign = IndexBits(i, 31, 31);
    return SignExtend(imm, sign, 11);
}

uint32_t S_Imm(uint32_t i) {
    uint32_t imm = IndexBits(i, 7, 11) | (IndexBits(i, 25, 30) << 5);
    uint32_t sign = IndexBits(i, 31, 31);
    return SignExtend(imm, sign, 11);
}

uint32_t B_Imm(uint32_t i) {
    uint32_t imm = 
        (IndexBits(i, 8, 11) << 1) | 
        (IndexBits(i, 25, 30) << 5) | 
        (IndexBits(i, 7, 7) << 11);

    uint32_t sign = IndexBits(i, 31, 31);
    return SignExtend(imm, sign, 12);
}

uint32_t U_Imm(uint32_t i) {
    uint32_t imm = (IndexBits(i, 12, 31) << 12);
    return imm;
}

uint32_t J_Imm(uint32_t i) {
    uint32_t imm =
        (IndexBits(i, 21, 24) << 1) |
        (IndexBits(i, 25, 30) << 5) |
        (IndexBits(i, 20, 20) << 11) |
        (IndexBits(i, 12, 19) << 12);

    uint32_t sign = IndexBits(i, 31, 31);
    return SignExtend(imm, sign, 20);
}

uint8_t Opcode(uint32_t i) {
    return (uint8_t) IndexBits(i, 0, 6);
}

uint8_t Funct3(uint32_t i) {
    return (uint8_t) IndexBits(i, 12, 14);
}

uint8_t Funct7(uint32_t i) {
    return (uint8_t) IndexBits(i, 25, 31);
}

uint8_t Rd(uint32_t i) {
    return (uint8_t) IndexBits(i, 7, 11);
}

uint8_t Rs1(uint32_t i) {
    return (uint8_t) IndexBits(i, 15, 19);
}

uint8_t Rs2(uint32_t i) {
    return (uint8_t) IndexBits(i, 20, 24);
}

DecodingNode* NewNode() {
    DecodingNode* dn = malloc(sizeof(DecodingNode));
    memset(dn, 0, sizeof(DecodingNode));
    return dn;
}

void AddInstruction(DecodingNode* root, const char* pattern, InstructionFunction insfn) {
    DecodingNode* curNode = root;

    for (int i = 31; i >= 0; i--) {
        char curBit = pattern[i];
        curNode->bit = i;

        if (curBit == '0') {
            if (curNode->Left == NULL) {
                curNode->Left = (struct DecodingNode*) NewNode();
            }

            curNode = (DecodingNode*) curNode->Left;

        } else if (curBit == '1') {
            if (curNode->Right == NULL) {
                curNode->Right = (struct DecodingNode*) NewNode();
            }

            curNode = (DecodingNode*) curNode->Right;
        } else {
            if (curNode->Neutral == NULL) {
                curNode->Neutral = (struct DecodingNode*) NewNode();
            }

            curNode = (DecodingNode*) curNode->Neutral;
        }
    }

    curNode->insFn = insfn;
}

InstructionFunction Lookup(DecodingNode* dn, uint32_t ins) {
    for (int i = 0; i < 32; i++) {
        uint8_t bit = IndexBit(ins, i);

        if (dn->Left != NULL && bit == 0) {
            dn = (DecodingNode*) dn->Left;
        } else if (dn->Right != NULL && bit == 1) {
            dn = (DecodingNode*) dn->Right;
        } else if (dn->Neutral != NULL) {
            dn = (DecodingNode*) dn->Neutral;
        } else {
            printf("Panic! At the incomplete decoding tree (probably an illegal instruction)\n");
            printf("Instruction: 0x%08X\n", ins);
            exit(1);
        }
    }

    return dn->insFn;
}

void FreeNode(DecodingNode* n) {
    if (n->Left != NULL) {
        FreeNode((DecodingNode*) n->Left);
    } else if (n->Neutral != NULL) {
        FreeNode((DecodingNode*) n->Neutral);
    } else if (n->Right != NULL) {
        FreeNode((DecodingNode*) n->Right);
    }

    free(n);
}
