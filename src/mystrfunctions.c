#include "../include/mystrfunctions.h"

// Returns the length of s, not counting the null terminator
int mystrlen(const char* s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// Copies src into dest, including the null terminator
// Returns the number of characters copied
int mystrcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}

// Copies at most n characters from src into dest
// If src is shorter than n, remaining bytes in dest are padded with '\0'
int mystrncpy(char* dest, const char* src, int n) {
    int i = 0;
    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    int copied = i;
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
    return copied;
}

// Appends src to the end of dest
// Returns the new total length of dest
int mystrcat(char* dest, const char* src) {
    int dest_len = mystrlen(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';
    return dest_len + i;
}
