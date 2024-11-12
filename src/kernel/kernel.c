#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot2.h"
#include "io.c"

void debug(uint8_t value)
{
#define PORT 0xe9
    __asm__ volatile("outb %b0, %w1" : : "a"(value), "Nd"(PORT) : "memory");
}

void debug2(char value[], uint32_t size)
{
    for (uint32_t i = 0; i < size - 1; i++)
    {
        debug(value[i]);
    }
}

void debug_num(unsigned int num)
{
    switch (num)
    {
    case 0:
        debug('0');
        break;
    case 1:
        debug('1');
        break;
    case 2:
        debug('2');
        break;
    case 3:
        debug('3');
        break;
    case 4:
        debug('4');
        break;
    case 5:
        debug('5');
        break;
    case 6:
        debug('6');
        break;
    case 7:
        debug('7');
        break;
    case 8:
        debug('8');
        break;
    case 9:
        debug('9');
        break;
    case 10:
        debug2("10", 3);
        break;
    case 11:
        debug2("11", 3);
        break;
    case 12:
        debug2("12", 3);
        break;
    case 13:
        debug2("13", 3);
        break;
    case 14:
        debug2("14", 3);
        break;
    case 15:
        debug2("15", 3);
        break;
    case 16:
        debug2("16", 3);
        break;
    case 17:
        debug2("17", 3);
        break;
    case 18:
        debug2("18", 3);
        break;
    case 19:
        debug2("19", 3);
        break;
    case 20:
        debug2("20", 3);
        break;
    case 21:
        debug2("21", 3);
        break;
    default:
        debug('?');
        break;
    }
}

char* framebuffer;
int scanline;

void kernel_main(unsigned long magic, unsigned long addr)
{
    char test[] = "Hello World!\n";
    debug2(test, sizeof(test) / sizeof(test[0]));

    // Verify multiboot2-compliant boot loader
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        debug2("xxx", 3);
    }

    // Verify alignment
    if (addr & 7)
    {
        debug2("yyy", 3);
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
        debug_num(tag->type);
        debug('\n');

        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            unsigned int color;
            unsigned i;
            multiboot_tag_framebuffer_t *tagfb = (multiboot_tag_framebuffer_t *)tag;
            void* fb = (void*)(unsigned long)tagfb->framebuffer_addr;
            framebuffer = (char*)tagfb->framebuffer_addr;
            scanline = tagfb->framebuffer_pitch;

            debug('(');
            debug_num(tagfb->framebuffer_type);
            debug(')');
            debug('\n');

            switch (tagfb->framebuffer_type)
            {
            case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                color = 0xFF0000;
                break;
            default:
                debug2("Unknown fb type.", 17);
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
                    debug2("A", 2);
                    break;
                }
            }
        }
        break;
        }
    }

    putchar(0x0048, 0, 0);  // H
    putchar(0x0065, 1, 0);  // e
    putchar(0x006C, 2, 0);  // l
    putchar(0x006C, 3, 0);  // l
    putchar(0x006F, 4, 0);  // o
    putchar(0x0020, 5, 0);  // -space-
    putchar(0x0057, 6, 0);  // W
    putchar(0x006F, 7, 0);  // o
    putchar(0x0072, 8, 0);  // r
    putchar(0x006C, 9, 0);  // l
    putchar(0x0064, 10, 0); // d
    putchar(0x0021, 10, 0); // !
}