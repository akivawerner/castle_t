/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-22-3.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-22-3.c -o CASTLE-22-3
vulnerable: true
description: Incorrectly implemented validation of a pathname leads to path traversal vulnerability.
cwe: 22
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// mkdir -p /var/www/html

int validatePath(char* str, const char* root) {
    if (strlen(str) < strlen(root)) {
        return 0;
    }
    return strncmp(str, root, strlen(root)) == 0;
}

int main(int argc, char *argv[]) {
    char file_name_buffer[PATH_MAX];
    const char public_root[] = "/var/www/html";

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    snprintf(file_name_buffer, sizeof(file_name_buffer), "%s/%s", public_root, argv[1]);

    // Validate path
    if (!validatePath(file_name_buffer, public_root)) {
        printf("Invalid path: %s", file_name_buffer);
        return 1;
    }

    // Print file content
    FILE *file = fopen(file_name_buffer, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    // Close the file
    fclose(file);

    return 0;
}