#include "testing.h"
#include "hart.h"
#include "instruction.h"

void TestRegister32() {
    Hart h; 

    WriteRegister32(&h, 0, 69);
    uint32_t v2 = ReadRegister32(&h, 0);
    TEST(v2 == 0, "Zero Register");

    WriteRegister32(&h, 1, 69);
    v2 = ReadRegister32(&h, 1);
    TEST(v2 == 69, "Other Registers");
}

void TestMaskBits() {
    uint32_t test = 0xFFFFFFFF;

    uint32_t result = MaskBits(test, 0, 7);
    uint32_t expect = 0xFF;
    TEST(result == expect, "Bit Index: 0-7");

    result = MaskBits(test, 3, 7);
    expect = 0xF8;
    TEST(result == expect, "Bit Index: 3-7");

    result = MaskBits(test, 31, 31);
    expect = 0x80000000;
    TEST(result == expect, "Bit Index: 31")
}

void TestExtractions() {
    uint32_t jal_x17_100 = 0x064008ef;
    uint32_t sra_x1_x17_x19 = 0x4138d0b3;

    uint8_t result = Opcode(jal_x17_100);
    uint8_t expect = 0x6F;
    TEST(result == expect, "Opcode Extraction");

    result = Rd(jal_x17_100);
    expect = 17;
    TEST(result == expect, "Rd Extraction");

    result = Funct3(sra_x1_x17_x19);
    expect = 5;
    TEST(result == expect, "Funct3 Extraction");

    result = Rs1(sra_x1_x17_x19);
    expect = 17;
    TEST(result == expect, "Rs1 Extraction");

    result = Rs2(sra_x1_x17_x19);
    expect = 19;
    TEST(result == expect, "Rs2 Exptraction");

    result = Funct7(sra_x1_x17_x19);
    expect = 0x20;
    TEST(result == expect, "Funct7 Extraction");
}

int main() {
    TestRegister32();
    TestMaskBits();
    TestExtractions();
    return 0;

}