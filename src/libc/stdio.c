#include "stdio.h"

// Returns the number of characters that were successfully printed
int printf(const char *format, ...)
{
    int len = 0;
    va_list params;
    va_start(params, format);
    bool skip = false;

    for (const char *c = format; *c != '\0'; c++)
    {
        if (skip)
        {
            skip = false;
            continue;
        }

        if (*c == '%')
        {
            skip = true;
            switch (*(c + 1))
            {
            case 'i':
            case 'd':
            {
                char num[11];
                memset(num, '\0', 11);

                intToString(va_arg(params, int), num);
                printStr(num);
                skip = true;
            }
            break;
            case 'c':
            {
                printChar((char)va_arg(params, int));
                skip = true;
            }
            break;
            case 's':
            {
                printStr(va_arg(params, char*));
                skip = true;
            }
            break;
            default:
            {
                printChar(*c);
                len++;
                skip = false;
            }
            break;
            }
        }
        else
        {
            printChar(*c);
            len++;
        }
    }

    va_end(params);

    return len;
}
