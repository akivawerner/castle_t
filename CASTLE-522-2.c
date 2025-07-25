/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-522-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-522-2.c -o CASTLE-522-2 -lsqlite3
vulnerable: true
description: Cleartext storage of password in database.
cwe: 522
============================================================================
*/

// create table users (username VARCHAR(32), password VARCHAR(32));

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sqlite3.h>

int isValidUsername(char *input) {
    for (int i = 0; i < strlen(input); i++) {
        if (!isalpha(input[i])) {
            return 0;
        }
    }
    return 1;
}

int isValidPassword(char *input) {
    for (int i = 0; i < strlen(input); i++) {
        if (!isalpha(input[i]) && !isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

int main() {
    sqlite3 *db;
    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Read user data
    char username[32];
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove trailing newline

    char password[32];
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // Remove trailing newline

    if (isValidUsername(username) == 0 || isValidPassword(password) == 0) {
        fprintf(stderr, "Username or password is not valid\n");
        sqlite3_close(db);
        return 1;
    }

    // Prepare an SQL query
    const char *sql = "INSERT INTO users (username, password) VALUES (?, ?);";

    // Compile the SQL query
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Bind values
    sqlite3_bind_text(res, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(res, 2, password, -1, SQLITE_STATIC);

    rc = sqlite3_step(res);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Record inserted successfully.\n");
    }

    // Finalize and clean up
    sqlite3_finalize(res);
    sqlite3_close(db);

    return 0;
}