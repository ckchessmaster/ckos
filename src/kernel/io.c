#include "io.h"
#include "font.h"

extern char* framebuffer;
extern int scanline; // Bytes per line (could be more than screen width)
extern char _binary_font_psf_start[];

const unsigned short unicodeMapSize = 95; // This constant should be updated as we add to the unicode map
unicode_lookup_t unicodeMap[] = 
{
    {' ', 0x0020}, {'!', 0x0021}, {'"', 0x0022}, {'#', 0x0023}, {'$', 0x0024}, {'%', 0x0025}, {'&', 0x0026}, {'\'', 0x0027}, {'(', 0x0028}, {')', 0x0029}, 
    {'*', 0x002A}, {'+', 0x002B}, {',', 0x002C}, {'-', 0x002D}, {'.', 0x002E}, {'/', 0x002F}, 
    {'0', 0x0030}, {'1', 0x0031}, {'2', 0x0032}, {'3', 0x0033}, {'4', 0x0034}, {'5', 0x0035}, {'6', 0x0036}, {'7', 0x0037}, {'8', 0x0038}, {'9', 0x0039}, 
    {':', 0x003A}, {';', 0x003B}, {'<', 0x003C}, {'=', 0x003D}, {'>', 0x003E}, {'?', 0x003F}, 
    {'@', 0x0040}, {'A', 0x0041}, {'B', 0x0042}, {'C', 0x0043}, {'D', 0x0044}, {'E', 0x0045}, {'F', 0x0046}, {'G', 0x0047}, {'H', 0x0048}, {'I', 0x0049}, 
    {'J', 0x004A}, {'K', 0x004B}, {'L', 0x004C}, {'M', 0x004D}, {'N', 0x004E}, {'O', 0x004F}, 
    {'P', 0x0050}, {'Q', 0x0051}, {'R', 0x0052}, {'S', 0x0053}, {'T', 0x0054}, {'U', 0x0055}, {'V', 0x0056}, {'W', 0x0057}, {'X', 0x0058}, {'Y', 0x0059}, 
    {'Z', 0x005A}, {'[', 0x005B}, {'\\', 0x005C}, {']', 0x005D}, {'^', 0x005E}, {'_', 0x005F}, 
    {'`', 0x0060}, {'a', 0x0061}, {'b', 0x0062}, {'c', 0x0063}, {'d', 0x0064}, {'e', 0x0065}, {'f', 0x0066}, {'g', 0x0067}, {'h', 0x0068}, {'i', 0x0069}, 
    {'j', 0x006A}, {'k', 0x006B}, {'l', 0x006C}, {'m', 0x006D}, {'n', 0x006E}, {'o', 0x006F}, 
    {'p', 0x0070}, {'q', 0x0071}, {'r', 0x0072}, {'s', 0x0073}, {'t', 0x0074}, {'u', 0x0075}, {'v', 0x0076}, {'w', 0x0077}, {'x', 0x0078}, {'y', 0x0079}, 
    {'z', 0x007A}, {'{', 0x007B}, {'|', 0x007C}, {'}', 0x007D}, {'~', 0x007E}
};

// Output a char to the qemu debugcon
void debugChar(char value)
{
    __asm__ volatile("outb %b0, %w1" : : "a"(value), "Nd"(DEBUG_PORT) : "memory");
}

// Output a string to the qemu debugcon
void debugString(char value[], int size)
{
    for (int i=0; i<size; i++)
    {
        debugChar(value[i]);
    }
}

void putchar(
    unsigned short int c, // This is an int not a char since it's a unicode character
    int cursorX,
    int cursorY)
{
    // cast the address to PSF header struct
    PSF2_Font_t *font = (PSF2_Font_t*)&_binary_font_psf_start;

    int bytesPerLine = (font->width + 7) / 8;

    // Get the glyph for the given character. If there's no glyph we'll display the first glyph.
    unsigned char* glyph = (unsigned char*)&_binary_font_psf_start + font->headerSize + (c > 0 && c < font->numGlyphs ? c : 0) * font->bytesPerGlyph;

    // Calculate the upper left corner of the screen.
    int offset = (cursorY * font->height * scanline) + (cursorX * (font->width + 1) * sizeof(PIXEL));

    // Display pixels according to the bitmap
    int x,y,line,mask;
    for (y=0; y<font->height; y++)
    {
        line = offset;
        mask = 1 << (font->width - 1);

        for (x=0; x<font->width; x++)
        {
            *((PIXEL*)(framebuffer + line)) = *((unsigned int*)glyph) & mask ? FONT_COLOR : 0;
            mask >>= 1;
            line += sizeof(PIXEL);
        }

        glyph += bytesPerLine;
        offset += scanline;
    }
}

unsigned short int getUnicodeValueFromChar(char c)
{
    unsigned short int value = UNICODE_UNKNOWN;

    for (int i=0; i<unicodeMapSize; i++)
    {
        if (unicodeMap[i].key == c)
        {
            value = unicodeMap[i].value;
            break;
        }
    }

    return value;
}