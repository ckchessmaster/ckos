#ifndef _STRING_H_
#define _STRING_H_

#include <stdbool.h>
#include <stddef.h>

// string.h libc functions
size_t strlen(const char* str);
char* strncpy(char* dest, const char* src, size_t n);
char* strcpy(char* dest, const char* src);

// Helper functions
char digitToChar(int digit);
unsigned int intToString(int i, char* string); // Returns the number of digits

#endif
