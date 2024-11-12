#include "font.h"

extern char* framebuffer;
extern int scanline; // Bytes per line (could be more than screen width)
extern char _binary_font_psf_start[];

#define PIXEL uint32_t

void debug3(uint8_t value)
{
#define PORT 0xe9
    __asm__ volatile("outb %b0, %w1" : : "a"(value), "Nd"(PORT) : "memory");
}

void putchar(
    unsigned short int c, // This is an int not a char since it's a unicode character
    int cursorX,
    int cursorY)
{
    // cast the address to PSF header struct
    PSF2_Font_t *font = (PSF2_Font_t*)&_binary_font_psf_start;
    if (font->magic != PSF2_FONT_MAGIC)
    {
        debug3('?');
        return;
    }

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