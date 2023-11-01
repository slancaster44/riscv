#include <stddef.h>

#include "exec.h"
#include "environment.h"
#include "decode.h"

void ExecIns(Hart* h) {
    uint32_t* mem = GetMemory();
    size_t pc_idx = h->PC.Integer64 / sizeof(uint32_t);

    uint32_t instruction = mem[pc_idx];
    h->PC.Integer64 += sizeof(uint32_t);

    uint8_t op = Opcode(instruction);
    switch (op) {
        
    }
}