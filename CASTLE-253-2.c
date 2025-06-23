/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-253-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-253-2.c -o CASTLE-253-2
vulnerable: true
description: Incorrect check of function return value behind a layer of abstraction
cwe: 253
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int* getIntPtr() {
    return malloc(sizeof(int));
}

int checkIntPtr(int* p) {
    return (long long)p % 2 == 1;
}

int main() {
    int* p = getIntPtr();
    if (checkIntPtr(p)) {
        printf("Failed to allocate memory.\n");
        return 1;
    }

    *p = 7;
    printf("Value: %d\n", *p);

    free(p);

    return 0;
}
