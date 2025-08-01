/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-401-6.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-401-6.c -o CASTLE-401-6
vulnerable: true
description: Memory is not released after effective lifetime, tricked by swap function called f
cwe: 401
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int square(int* a) {
    int b = *a * *a;
    return b;
}

void f(int** a, int** b) {
    int* temp = *a;
    *a = *b;
    *b = temp;
}

void safeFree(int** a) {
    free(*a);
    *a = NULL;
}

int main() {
    int *a = (int*)malloc(sizeof(int));
    int *b = (int*)malloc(sizeof(int));
    if (a == NULL || b == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    *a = 5;
    printf("Result: %d\n", square(a));

    *b = -7;
    printf("Result: %d\n", square(b));

    safeFree(&a);
    f(&a, &b);
    safeFree(&b);

    return 0;
}
