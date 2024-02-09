// All tests are licensed under the unlicense. Do whatever you want with these
// tests

#include <assert.h>
#include <stdlib.h>

#include "beanutils/beanutils.h"

#define RUNTEST(name, fun)                                                     \
    Bean_Log(LOGLEVEL_LOG, "Running test \"" name "\"");                       \
    fun()

void Test_areTestsWorking(void) { assert(1 != 0); }

void Test_reallocPointerAddresses(void) {
    void** firstptr = (void**)malloc(sizeof(void*) * 5);
    void** secptr = (void**)realloc(firstptr, sizeof(void*) * 6);

    assert(firstptr == secptr);
}

int main(void) {
    RUNTEST("Are tests working", Test_areTestsWorking);
    RUNTEST("realloc pointer addresses", Test_reallocPointerAddresses);
}
