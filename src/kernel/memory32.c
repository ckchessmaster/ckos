#include "memory32.h"
#include "stdio.h"

extern page_directory_entry page_directory[1024];
extern page_directory_table_entry page_table_1[1024];
extern uint64_t _startKernel;
extern uint64_t _endKernel;

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

uint64_t availableMemory = 0;
uint64_t totalMemory = 0;

memory_map_entry_t* mm;
memory_map_entry_t* newNode = 0;

void initMemory(multiboot_tag_memory_map_t* memoryMap)
{
    // Build the initial memory map
    unsigned int numEntries = (memoryMap->size - 16) / memoryMap->entry_size;
    
    // 1. Start by finding a place large enough to put our memory map (1MB will be the start)
    for (unsigned int i=0; i<numEntries - 1; i++)
    {
        multiboot_tag_memory_map_entry_t mapEntry = memoryMap->entries[i];        
        if (mapEntry.type == MULTIBOOT_MEMORY_AVAILABLE && mapEntry.length >= 1024 * 1024)
        {
            mm = (memory_map_entry_t*)(uintptr_t)mapEntry.base_addr;
            break;
        }
    }
    // 2. Now loop back through the multiboot map to build our own internal map
    memory_map_entry_t* current = mm;
    memory_map_entry_t* previous = NULL;
    for (unsigned int i=0; i<numEntries - 1; i++)
    {
        multiboot_tag_memory_map_entry_t mapEntry = memoryMap->entries[i];
        
        totalMemory += mapEntry.length;
        current->address = mapEntry.base_addr;

        if (mapEntry.type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            availableMemory += mapEntry.length;
            current->status = MEMORY_AVAILABLE;
        }
        else
        {
            current->status = MEMORY_FIRMWARE_RESERVED;
        }

        current->next = NULL;
        if (previous)
        {
            previous->next = current;
            current->previous = previous;
        }

        previous = current;
        current += sizeof(memory_map_entry_t);
        newNode = current;
    }
    // 3. Reserve the memory block used for the memory map
    current = mm;
    while (current)
    {
        // If we found it create a new node in the list
        if (current->address == (uint64_t)(uintptr_t)mm)
        {
            // Create new node
            newNode->previous = current;
            newNode->next = current->next;
            newNode->address = current->address + (1024 * 1024); // Reserve 1MB
            newNode->status = MEMORY_AVAILABLE;

            // Setup current node
            current->next = newNode;
            current->status = MEMORY_KERNEL_RESERVED;

            // Increment so we can continue to create new nodes later
            newNode += sizeof(memory_map_entry_t);

            break;
        }

        current = current->next;
    }

    // TODO: Re-enable paging
    // // Blank out all of the entries
    // for (unsigned int i=0; i<DIRECTORY_LENGTH; i++)
    // {
    //     page_directory_entry directoryEntry = 0;
    //     directoryEntry = directoryEntry | 0<<0; // Present bit
    //     directoryEntry = directoryEntry | 1<<1; // Write Enabled bit
    //     directoryEntry = directoryEntry | 0<<2; // Supervisor bit

    //     page_directory[i] = directoryEntry;
    // }

    // // Fill out the first page table mapping the first 4 MB of physical memory
    // for (unsigned int i=0; i<TABLE_LENGTH; i++)
    // {
    //     // Initialize the address bits (the last 12 bits will always be 0 since we are page aligned. These are used for the attributes)
    //     page_directory_table_entry tableEntry = i * PAGE_SIZE;

    //     // Set the attributes
    //     tableEntry = tableEntry | 1<<0; // Present bit
    //     tableEntry = tableEntry | 1<<1; // Write Enabled bit
    //     tableEntry = tableEntry | 0<<2; // Supervisor bit

    //     page_table_1[i] = tableEntry;
    // }

    // // Insert the first page table into the directory
    // page_directory[0] = ((page_directory_entry)page_table_1) | 3; // 3 adds the present, write, and supervisor bits

    // loadPageDirectory((unsigned int *)page_directory);
    // enablePaging();
}

void memoryDiagnostics()
{
    printf("Memory info:\n");
    char totalMemory_s[19];
    memset(totalMemory_s, '\0', 19);
    uint64ToString(totalMemory / 1024 / 1024, totalMemory_s);

    char availableMemory_s[19];
    memset(availableMemory_s, '\0', 19);
    uint64ToString(availableMemory / 1024 / 1024, availableMemory_s);

    printf("Total: %sMB Available: %sMB\n", totalMemory_s, availableMemory_s);

    printf("Page directory: %d Page table: %d\n", page_directory[0], page_table_1[0]);

    char kstart[19];
    memset(kstart, '\0', 19);
    uint64ToString(_startKernel, kstart);

    char kend[19];
    memset(kend, '\0', 19);
    uint64ToString(_endKernel, kend);
    printf("Kernel start: %s Kernel end: %s\n", kstart, kend);

    printf("\nMemory Map:\n");
    memory_map_entry_t* current = mm;
    unsigned int i=0;
    while (current)
    {
        // TODO: For some reason the last address ends up being greater than the max memory
        char length_s[19];
        memset(length_s, '\0', 19);
        uint64_t length = current->next 
            ? (current->next->address - current->address) / 1024 
            : totalMemory > current->address ? (totalMemory - current->address) / 1024 : 0;
        uint64ToString(length, length_s);

        char addr[19];
        memset(addr, '\0', 19);
        uint64ToString(current->address, addr);

        printf("Entry: %d Type: %d Length: %s KB Address: %s\n", i, current->status, length_s, addr);

        current = current->next;
        i++;
    }

    printf("--------------------------------\n\n");
}