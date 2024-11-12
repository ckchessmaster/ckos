#ifndef _FONT_H_
#define _FONT_H_

#include <stddef.h>
#include <stdint.h>

#define PSF1_FONT_MAGIC 0x0436
#define PSF2_FONT_MAGIC 0x864ab572

#define FONT_COLOR 0x00FF00 // Green

typedef struct PSF1_Header {
    uint16_t magic;
    uint8_t fontMode;
    uint8_t characterSize;
} PSF1_Header_t;

typedef struct PSF2_Font {
    uint32_t magic;
    uint32_t version;       // 0
    uint32_t headerSize;    // Offset of bitmaps in file
    uint32_t flags;         // 0 if there's no unicode table
    uint32_t numGlyphs;
    uint32_t bytesPerGlyph;
    uint32_t height;
    uint32_t width;
} PSF2_Font_t;
#endif