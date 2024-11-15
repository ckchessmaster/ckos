#include "debug.h"

// Output a char to the qemu debugcon
void debugChar(const char c)
{
    __asm__ volatile("outb %b0, %w1" : : "a"(c), "Nd"(DEBUG_PORT) : "memory");
}

// Output a string to the qemu debugcon
void debugString(const char* s)
{
    for (const char* c=s; *c != '\0'; c++)
    {
        debugChar(*c);
    }
}
