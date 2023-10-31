#ifndef TESTING_H
#define TESTING_H

void Fail(const char* msg);
void Pass(const char* msg);
void PrintTestSummary();

#define TEST(CONDITION, TEST_MSG) { \
    if (CONDITION) {                \
        Pass(TEST_MSG);             \
    } else {                        \
        Fail(TEST_MSG);             \
    }                               \
}                                   \

#endif