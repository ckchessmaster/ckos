#define PIXEL uint32_t
#define DEBUG_PORT 0xe9
#define UNICODE_UNKNOWN 0xFFFD

typedef struct unicode_lookup
{
    char key;
    unsigned short int value;
} unicode_lookup_t;

void debugChar(char value);
void debugString(char value[], int size);
void putchar(unsigned short int c, int cursorX, int cursorY);
unsigned short int getUnicodeValueFromChar(char c);