#include "testing.h"
#include <stdio.h>

static int num_failed;
static int num_passed;

void Fail(const char* msg) {
    num_failed ++;
    printf("\033[0;31m[FAIL]\033[0;37m %s\n", msg);
}

void Pass(const char* msg) {
    num_passed ++;
    printf("\033[0;32m[PASS]\033[0;37m %s\n", msg);
}

void PrintTestSummary() {
    printf("Test(s) Passed: %d\nTests(s) Failed: %d\n", num_passed, num_failed);
}