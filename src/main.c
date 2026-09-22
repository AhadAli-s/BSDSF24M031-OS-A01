#include <stdio.h>
#include <stdlib.h>
#include "../include/mystrfunctions.h"
#include "../include/myfilefunctions.h"

int main() {
    printf("--- Testing String Functions ---\n");

    printf("mystrlen(\"hello\") = %d\n", mystrlen("hello"));

    char dest1[50];
    mystrcpy(dest1, "copied string");
    printf("mystrcpy result: %s\n", dest1);

    char dest2[50];
    mystrncpy(dest2, "truncate me", 5);
    dest2[5] = '\0';
    printf("mystrncpy result (5 chars): %s\n", dest2);

    char dest3[50] = "Hello, ";
    mystrcat(dest3, "World!");
    printf("mystrcat result: %s\n", dest3);

    printf("\n--- Testing File Functions ---\n");

    FILE* fp = fopen("test.txt", "r");
    if (fp == NULL) {
        printf("Could not open test.txt, skipping file tests.\n");
        return 1;
    }

    int lines, words, chars;
    if (wordCount(fp, &lines, &words, &chars) == 0) {
        printf("wordCount -> lines: %d, words: %d, chars: %d\n", lines, words, chars);
    }

    rewind(fp);

    char** matches;
    int matchCount = mygrep(fp, "test", &matches);
    printf("mygrep found %d matching line(s):\n", matchCount);
    for (int i = 0; i < matchCount; i++) {
        printf("  %s", matches[i]);
        free(matches[i]);
    }
    free(matches);

    fclose(fp);
    return 0;
}
