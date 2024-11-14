#include "string.h"

size_t strlen(const char* str)
{
    size_t length = 0;
    for (const char* c=str; *c != '\0'; c++)
    {
        length++;
    }

    return length;
}
