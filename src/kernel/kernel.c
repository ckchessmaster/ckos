#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "debug.h"
#include "memory32.h"
#include "multiboot2.h"
#include "stdio.h"

uint64_t framebuffer;
unsigned int scanline;
unsigned int screenHeight;
unsigned int screenWidth;

void kernel_main(unsigned long magic, unsigned long addr);
void test();

void kernel_main(unsigned long magic, unsigned long addr)
{
    debugString("Kernel initializing...\n");

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
    debugString("Bootloader validated.\n");

    multiboot_tag_t *tag;
    for (tag = (multiboot_tag_t *)(addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (multiboot_tag_t *)((uint8_t *)tag + ((tag->size + 7) & 4294967288)))
    {
        switch (tag->type)
        {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
            {
                multiboot_tag_framebuffer_t* fbTag = (multiboot_tag_framebuffer_t*)tag;

                initTerminal(fbTag);
                debugString("Display initialized.\n");
            }
            break;
            case MULTIBOOT_TAG_TYPE_MEMORY_MAP:
            {
                multiboot_tag_memory_map_t* mmTag = (multiboot_tag_memory_map_t*)tag;
                initMemory(mmTag);
                debugString("Memory initialized.\n");
            }
            break;
        }
    }
    debugString("Kernel initialization complete!\n");

    cls();
    printf("Welcome to CKOS!\n--------------------------------\n");
    displayDiagnostics();
    memoryDiagnostics();
    displayTest();
}
