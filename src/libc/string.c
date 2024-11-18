#include "string.h"

void* memset(void *str, int c, size_t n)
{
    unsigned char fillByte = (unsigned char)c;
    unsigned char* temp=str;
    for (size_t i=0; i<n; i++)
    {
        temp[i] = fillByte;
    }

    return str;
}

size_t strlen(const char* str)
{
    size_t length = 0;
    for (const char* c=str; *c != '\0'; c++)
    {
        length++;
    }

    return length;
}

char* strncpy(char* dest, const char* src, size_t n)
{
    bool endFound = false;
    for (size_t i=0; i<n; i++)
    {
        if (endFound)
        {
            dest[i] = '\0';
        }
        else
        {
            dest[i] = src[i];
            if (src[i] == '\0')
            {
                endFound = true;
            }
        }
    }

    return dest;
}

char* strcpy(char* dest, const char* src)
{
    size_t srcLength = strlen(src);
    return strncpy(dest, src, srcLength);
}

unsigned int intToString(int value, char* string)
{
    bool isNegative = value < 0;
    unsigned int numDigits = 0;
    
    // Convert to positive so we can use mod properly
    if (isNegative)
    {
        value = value * -1;
        string[0] = '-';
    }

    int modVal = 1000000000;
    int divide = 100000000;
    int stringIndex = isNegative ? 1 : 0;
    bool startFound = false;

    for (short int i=0; i<9; i++)
    {
        int digit = (value % modVal) / divide;
        if (startFound || digit != 0)
        {
            if (!startFound)
            {
                startFound = true;
            }

            string[stringIndex] = digitToChar(digit);
            stringIndex++;
            numDigits++;
        }        

        modVal = modVal / 10;
        divide = divide / 10;
    }

    return numDigits;
}

char digitToChar(int digit)
{
    switch (digit)
    {
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        case 0:
        default:
            return '0';
    }
}
