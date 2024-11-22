#include "memory32.h"
#include "stdio.h"

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();
extern page_directory_entry page_directory[1024];
extern page_directory_table_entry page_table_1[1024];

void initMemory()
{
    // Blank out all of the entries
    for (unsigned int i=0; i<DIRECTORY_LENGTH; i++)
    {
        page_directory_entry directoryEntry = 0;
        directoryEntry = directoryEntry | 0<<0; // Present bit
        directoryEntry = directoryEntry | 1<<1; // Write Enabled bit
        directoryEntry = directoryEntry | 0<<2; // Supervisor bit

        page_directory[i] = directoryEntry;
    }

    // Fill out the first page table mapping the first 4 MB of physical memory
    for (unsigned int i=0; i<TABLE_LENGTH; i++)
    {
        // Initialize the address bits (the last 12 bits will always be 0 since we are page aligned. These are used for the attributes)
        page_directory_table_entry tableEntry = i * PAGE_SIZE;

        // Set the attributes
        tableEntry = tableEntry | 1<<0; // Present bit
        tableEntry = tableEntry | 1<<1; // Write Enabled bit
        tableEntry = tableEntry | 0<<2; // Supervisor bit

        page_table_1[i] = tableEntry;
    }

    // Insert the first page table into the directory
    page_directory[0] = ((page_directory_entry)page_table_1) | 3; // 3 adds the present, write, and supervisor bits

    loadPageDirectory((unsigned int *)page_directory);
    enablePaging();
}

void allocatePages(unsigned int n)
{
    printf("Page directory: %d\n", page_directory[0]);
    printf("Page table: %d\n", page_table_1[0]);
}