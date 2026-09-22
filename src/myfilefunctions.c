
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/myfilefunctions.h"

// Counts lines, words, and characters in the given file
// Returns 0 on success, -1 on failure
int wordCount(FILE* file, int* lines, int* words, int* chars) {
    if (file == NULL) {
        return -1;
    }

    *lines = 0;
    *words = 0;
    *chars = 0;

    int c;
    int in_word = 0;

    while ((c = fgetc(file)) != EOF) {
        (*chars)++;

        if (c == '\n') {
            (*lines)++;
        }

        if (c == ' ' || c == '\n' || c == '\t') {
            in_word = 0;
        } else if (in_word == 0) {
            in_word = 1;
            (*words)++;
        }
    }

    return 0;
}

// Searches for lines in fp containing search_str
// Allocates and fills matches with pointers to the matching lines
// Returns the number of matches, or -1 on failure
int mygrep(FILE* fp, const char* search_str, char*** matches) {
    if (fp == NULL || search_str == NULL) {
        return -1;
    }

    int capacity = 10;
    int count = 0;
    char** results = malloc(capacity * sizeof(char*));
    if (results == NULL) {
        return -1;
    }

    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, search_str) != NULL) {
            if (count == capacity) {
                capacity *= 2;
                char** temp = realloc(results, capacity * sizeof(char*));
                if (temp == NULL) {
                    free(results);
                    return -1;
                }
                results = temp;
            }

            results[count] = malloc(strlen(buffer) + 1);
            if (results[count] == NULL) {
                free(results);
                return -1;
            }
            strcpy(results[count], buffer);
            count++;
        }
    }

    *matches = results;
    return count;
}
