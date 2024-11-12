#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot2.h"
#include "io.h"

char *framebuffer;
int scanline;

void kernel_main(unsigned long magic, unsigned long addr)
{
    char test[] = "Hello World!\n";
    debugString(test, sizeof(test) / sizeof(test[0]));

    // Verify multiboot2-compliant boot loader
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        debugString("xxx", 3);
    }

    // Verify alignment
    if (addr & 7)
    {
        debugString("yyy", 3);
    }

    multiboot_tag_t *tag;
    unsigned size;

#define red 0xFF0000
#define green 0x00FF00
#define blue 0x0000FF

    size = *(unsigned *)addr;
    for (tag = (multiboot_tag_t *)(addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (multiboot_tag_t *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            unsigned int color;
            unsigned i;
            multiboot_tag_framebuffer_t *tagfb = (multiboot_tag_framebuffer_t *)tag;
            void *fb = (void *)(unsigned long)tagfb->framebuffer_addr;
            framebuffer = (char *)tagfb->framebuffer_addr;
            scanline = tagfb->framebuffer_pitch;

            switch (tagfb->framebuffer_type)
            {
            case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                color = 0xFF0000;
                break;
            default:
                debugString("Unknown fb type.", 16);
                color = red;
                break;
            }

            for (i = 0; i < tagfb->framebuffer_width && i < tagfb->framebuffer_height; i++)
            {
                switch (tagfb->framebuffer_bpp)
                {
                case 32:
                {
                    uint32_t *pixel = fb + tagfb->framebuffer_pitch * i + 4 * i;
                    *pixel = color;
                }
                break;
                default:
                    debugString("A", 1);
                    break;
                }
            }
        }
        break;
        }
    }

    putchar(getUnicodeValueFromChar('H'), 0, 0);
    putchar(getUnicodeValueFromChar('e'), 1, 0);
    putchar(getUnicodeValueFromChar('l'), 2, 0);
    putchar(getUnicodeValueFromChar('l'), 3, 0);
    putchar(getUnicodeValueFromChar('o'), 4, 0);
    putchar(getUnicodeValueFromChar(' '), 5, 0);
    putchar(getUnicodeValueFromChar('W'), 6, 0);
    putchar(getUnicodeValueFromChar('o'), 7, 0);
    putchar(getUnicodeValueFromChar('r'), 8, 0);
    putchar(getUnicodeValueFromChar('l'), 9, 0);
    putchar(getUnicodeValueFromChar('d'), 10, 0);
    putchar(getUnicodeValueFromChar('!'), 11, 0);
}