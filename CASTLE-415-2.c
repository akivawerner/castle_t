/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-415-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-415-2.c -o CASTLE-415-2
vulnerable: true
description: Freeing memory again after it has been freed, tricked by swap function
cwe: 415
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int square(int* a) {
    int b = *a * *a;
    free(a);
    return b;
}

void swap(int** a, int** b) {
    int* temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int *a = (int*) malloc(sizeof(int));

    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    *a = 5;
    int b = square(a);
    printf("Result: %d\n", b);

    int *p = &b;
    swap(&a, &p);
    if (b > 20) {
        free(p);
    }

    return 0;
}
