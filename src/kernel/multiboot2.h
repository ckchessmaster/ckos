#include <stdint.h>

/*  This should be in %eax. */
#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289

#define MULTIBOOT_TAG_TYPE_END 0
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8

typedef struct multiboot_tag
{
    uint32_t type;
    uint32_t size;
} multiboot_tag_t;

typedef struct multiboot_multiboot_tag_framebuffer_palette
{
    uint8_t red_value;
    uint8_t green_value;
    uint8_t blue_value;
} multiboot_multiboot_tag_framebuffer_palette_t;

typedef struct multiboot_tag_framebuffer
{
    uint32_t type; // Always 8
    uint32_t size;

    uint64_t framebuffer_addr; // Physical address of the framebuffer
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp; // Bits per pixel ie. 8, 16, 32, etc.

#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB 1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2
    uint8_t framebuffer_type;
    uint16_t reserved; // Always 0 and should not be used

    union
    {
        struct
        {
            uint16_t framebuffer_pallete_num_colors;
            multiboot_multiboot_tag_framebuffer_palette_t framebuffer_pallete[0];
        };
        struct
        {
            uint8_t framebuffer_red_field_position;
            uint8_t framebuffer_red_mask_size;
            uint8_t framebuffer_green_field_position;
            uint8_t framebuffer_green_mask_size;
            uint8_t framebuffer_blue_field_position;
            uint8_t framebuffer_blue_mask_size;
        };
    };
} multiboot_tag_framebuffer_t;