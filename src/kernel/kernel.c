#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot2.h"
#include "debug.h"
#include "stdio.h"

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();
extern unsigned int* page_directory;

uint64_t framebuffer;
unsigned int scanline;
unsigned int screenHeight;
unsigned int screenWidth;

void kernel_main(unsigned long magic, unsigned long addr);
void displayDiagnostics();
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

    //loadPageDirectory(page_directory);
    //enablePaging();
    //debugString("Paging loaded.\n");

    multiboot_tag_t *tag;
    for (tag = (multiboot_tag_t *)(addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (multiboot_tag_t *)((uint8_t *)tag + ((tag->size + 7) & 4294967288)))
    {
        switch (tag->type)
        {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
            {
                multiboot_tag_framebuffer_t *tagfb = (multiboot_tag_framebuffer_t *)tag;
                framebuffer = tagfb->framebuffer_addr;
                scanline = tagfb->framebuffer_pitch;
                screenHeight = tagfb->framebuffer_height;
                screenWidth = tagfb->framebuffer_height;
            }
            break;
        }
    }
    debugString("Display initialized.\n");
    debugString("Kernel initialization complete!\n");

    displayDiagnostics();
    test();
}

void displayDiagnostics()
{
    printf("Welcome to CKOS!\n--------------------------------\n");
    printf("Display Info:\nH: %d W: %d L: %d", screenHeight, screenWidth, scanline);
    printf("\n--------------------------------\n\n");
}

void test()
{
    const char* src = "Hello World2!\n";
    char dst[15];
    memset(dst, '\0', 15);

    char* final = strncpy(dst, src, 7);
    printf(final);
    printf("\n");

    printf("Letter is %c!\n", 'A');
    printf("String is %s!\n", "cat");
    printf("Number is: %d!\n", 123);
    printf("Number is: %i!\n", 4567890);

    printf("Number is: %d! Next number is: %d!\n", 123, 456);
}
