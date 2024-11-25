#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

#include "multiboot2.h"

#define DIRECTORY_LENGTH 1024
#define TABLE_ENTRY_SIZE 4096
#define TABLE_LENGTH 1024
#define PAGE_SIZE 4096

// Bit Map:
// 0 - P (Present): 1 = Page is in physical memory, 0 = Page is not in physical memory
// 1 - R/W (Read/Write): 1 = Page is allows read/write, 0 = Page is read-only
// 2 - U/S (User/Supervised): 1 = Page is accessible to users, 0 = Page is only accessible to supervisors
// 3 - PWT: 1 = write-through caching is enabled, 0 = write-back is enabled
// 4 - PCD: 1 = Caching is disabled, 0 = Caching is enabled
// 5 - A (Accessed): 1 = PDE or PTD was read during virtual address translation. (Not auto-cleared by the CPU)
// 6 - AVL (Available): Free bits to be used by the OS
// 7 - PS (Page Size): 0 = The address points to a 4KB aligned page table, 1 = The address points directly to a 4MB algined page
// 8-11 - AVL (Available): Free bits to be used by the OS
// 12-31 - Bits 31-12 of address
typedef uint32_t page_directory_entry;

// Bit Map:
// 0 - P (Present): 1 = Page is in physical memory, 0 = Page is not in physical memory
// 1 - R/W (Read/Write): 1 = Page is allows read/write, 0 = Page is read-only
// 2 - U/S (User/Supervised): 1 = Page is accessible to users, 0 = Page is only accessible to supervisors
// 3 - PWT: 1 = write-through caching is enabled, 0 = write-back is enabled
// 4 - PCD: 1 = Caching is disabled, 0 = Caching is enabled
// 5 - A (Accessed): 1 = PDE or PTD was read during virtual address translation. (Not auto-cleared by the CPU)
// 6 - D (Dirty): 1 = Page has been written to, 0 = Page has not been written to
// 7 - PS (Page Size): 0 = The address points to a 4KB aligned page table, 1 = The address points directly to a 4MB algined page
// 8 - G (Global): 1 = Tells the processor not to invalidate the TLB entry corresponding to the page upon a MOV to CR3 instruction (Note: bit 7 (PGE) in CR4 must be set to enable global pages)
// 9-11 - AVL (Available): Free bits to be used by the OS
// 12 - PAT (Page Attribute Table): If PAT is supported then PAT along with PCD & PWT shall indicate the memory caching type. Otherwise it is reserved and must be set to 0
// 13-20 - Bits 39-32 of address
// 21 - RSVD
// 22-31 - Bits 31-22 of address
typedef uint32_t page_directory_entry_4MB;

// Bit Map:
// 0 - P (Present): 1 = Page is in physical memory, 0 = Page is not in physical memory
// 1 - R/W (Read/Write): 1 = Page is allows read/write, 0 = Page is read-only
// 2 - U/S (User/Supervised): 1 = Page is accessible to users, 0 = Page is only accessible to supervisors
// 3 - PWT: 1 = write-through caching is enabled, 0 = write-back is enabled
// 4 - PCD: 1 = Caching is disabled, 0 = Caching is enabled
// 5 - A (Accessed): 1 = PDE or PTD was read during virtual address translation. (Not auto-cleared by the CPU)
// 6 - D (Dirty): 1 = Page has been written to, 0 = Page has not been written to
// 7 - PAT (Page Attribute Table): If PAT is supported then PAT along with PCD & PWT shall indicate the memory caching type. Otherwise it is reserved and must be set to 0
// 8 - G (Global): 1 = Tells the processor not to invalidate the TLB entry corresponding to the page upon a MOV to CR3 instruction (Note: bit 7 (PGE) in CR4 
// 9-11 - AVL (Available): Free bits to be used by the OS
// 12-31 - Bits 31-12 of address
typedef uint32_t page_directory_table_entry;

typedef struct memory_map_entry
{
    struct memory_map_entry* previous;
    struct memory_map_entry* next;
    uint64_t address;
#define MEMORY_AVAILABLE 1          // Available memory
#define MEMORY_USED 2               // In use by non-critical paths
#define MEMORY_FIRMWARE_RESERVED 3  // In use by firmware, this can never be freed
#define MEMORY_KERNEL_RESERVED 4    // In use by the kernel for critical paths, can only be freed by the kernel itself
    uint8_t status;
} memory_map_entry_t;

void initMemory(multiboot_tag_memory_map_t* memoryMap);
void memoryDiagnostics();

#endif
