#ifndef _STRING_H_
#define _STRING_H_

#include <stdbool.h>
#include <stddef.h>

// string.h libc functions
void* memset(void *str, int c, size_t n);
size_t strlen(const char* str);
char* strncpy(char* dest, const char* src, size_t n);
char* strcpy(char* dest, const char* src);

// Helper functions
char digitToChar(int digit);
unsigned int intToString(int i, char* outString); // Returns the number of digits
unsigned int int64ToString(long long int value, char* outString); // Returns the number of digits
unsigned int uint64ToString(unsigned long long int value, char* outString);

#endif
