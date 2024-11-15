#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include "font.h"

#define PIXEL uint32_t
#define UNICODE_UNKNOWN 0xFFFD

typedef struct unicode_lookup
{
    char key;
    unsigned short int value;
} unicode_lookup_t;


unsigned short int getUnicodeValueFromChar(char c);
void putchar(
    unsigned short int c, // This is an int not a char since it's a unicode character
    unsigned int cursorX,
    unsigned int cursorY);
void printk(const char* s);

#endif