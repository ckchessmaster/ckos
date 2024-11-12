#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot2.h"
#include "io.h"

char *framebuffer;
int scanline;

void kernel_main(unsigned long magic, unsigned long addr)
{
    debugString("Hello World!\n");

    // Verify multiboot2-compliant boot loader
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        debugString("Invalid magic number detected!");
        return;
    }

    // Verify alignment
    if (addr & 7)
    {
        debugString("Invalid header alignment detected!");
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

    printk("Hello World!");
}