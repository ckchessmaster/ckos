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

void initTerminal(
    uint64_t framebufferAddress, 
    unsigned int screenHeight, 
    unsigned int screenWidth,
    unsigned int scanline);
unsigned short int getUnicodeValueFromChar(char c);
void putchar(
    unsigned short int c, // This is an int not a char since it's a unicode character
    unsigned int cursorX,
    unsigned int cursorY);
int printStr(const char* s);
void printChar(char c);

#endif