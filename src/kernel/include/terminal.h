#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include "font.h"
#include "multiboot2.h"
#include "stdio.h"

#define PIXEL uint32_t
#define UNICODE_UNKNOWN 0xFFFD

typedef struct unicode_lookup
{
    char key;
    unsigned short int value;
} unicode_lookup_t;

void initTerminal(multiboot_tag_framebuffer_t* tag);
unsigned short int getUnicodeValueFromChar(char c);

int printStr(const char* s);
void printChar(char c);
void cls();
void displayDiagnostics();

#endif