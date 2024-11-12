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
        return;
    }

    // Verify alignment
    if (addr & 7)
    {
        debugString("yyy", 3);
        return;
    }

    multiboot_tag_t *tag;
    unsigned size;

    size = *(unsigned *)addr;
    for (tag = (multiboot_tag_t *)(addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (multiboot_tag_t *)((uint8_t *)tag + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            multiboot_tag_framebuffer_t *tagfb = (multiboot_tag_framebuffer_t *)tag;
            framebuffer = (char *)tagfb->framebuffer_addr;
            scanline = tagfb->framebuffer_pitch;
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