#include "hart.h"
#include <string.h>

uint32_t ReadRegister32(Hart* h, uint8_t register_number) {
    uint32_t output;

    if (register_number == 0) {
        output = 0;
    } else {
        output = h->registers[register_number-1].Integer32;
    } 

    return output;
}

void  WriteRegister32(Hart* h, uint8_t register_number, uint32_t v) {
    if (register_number != 0) {
        h->registers[register_number-1].Integer32 = v;
    }
}