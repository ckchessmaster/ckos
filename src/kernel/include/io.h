#ifndef _IO_H_
#define _IO_H_

#define PIXEL uint32_t
#define DEBUG_PORT 0xe9
#define UNICODE_UNKNOWN 0xFFFD

typedef struct unicode_lookup
{
    char key;
    unsigned short int value;
} unicode_lookup_t;

void debugChar(const char c);
void debugString(const char* s);
void putchar(unsigned short int c, unsigned int cursorX, unsigned int cursorY);
unsigned short int getUnicodeValueFromChar(char c);
void printk(const char* s);

#endif