#include "testing.h"
#include "hart.h"
#include "decoding.h"
#include "environment.h"
#include "exec.h"

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

void TestImmediates() {
    uint32_t result = SignExtend(0x3, 0, 25); 
    uint32_t expect = 0x3;
    TEST(result == expect, "Zero extend");

    result = SignExtend(0x3, 1, 25);
    expect = 0xFE000003;
    TEST(result == expect, "Sign extend");

    uint64_t result64 = SignExtend64(0xF);
    uint64_t expect64 = 0xF;
    TEST(result64 == expect64, "Zero extend 64");

    result64 = SignExtend64(0xFFFFFFFF);
    expect64 = (uint64_t) -1;
    TEST(result64 == expect64, "Sign extend 64");

    uint32_t addi_x1_x0_neg1 = 0xfff00093;
    result = I_Imm(addi_x1_x0_neg1);
    expect = (unsigned) -1;
    TEST(result == expect, "I_Imm Extraction");

    uint32_t sb_x17_neg1_x19 = 0xff198fa3;
    result = S_Imm(sb_x17_neg1_x19);
    expect = (unsigned) -1;
    TEST(result == expect, "S_Imm Extraction");

    uint32_t bne_x17_x19_neg2 = 0xff389fe3;
    result = B_Imm(bne_x17_x19_neg2);
    expect = (unsigned) -2;
    TEST(result == expect, "B_Imm Extraction");

    uint32_t lui_x17_neg1 = 0xfffff8b7;
    result = U_Imm(lui_x17_neg1);
    expect = 0xFFFFF000;
    TEST(result == expect, "U_Imm Extraction");

    uint32_t jal_x17_neg2 = 0xfffff8ef;
    result = J_Imm(jal_x17_neg2);
    expect = (unsigned) -2;
    TEST(result == expect, "J_Imm Extraction");

}

void TestInstructions() {
    Hart h = NewHart();
    uint32_t code[] = {0xfffff8b7, 0xfff00093,  0x4138d0b3};
    InitializeMemory(&code[0], sizeof(code));

    for (int i = 0; i < (sizeof(code) / sizeof(code[0])); i++) {
        ExecIns(&h);
    }    

    CleanupMemory();
}

int main() {
    TestRegister32();
    TestMaskBits();
    TestExtractions();
    TestImmediates();
    TestInstructions();
    return 0;

}