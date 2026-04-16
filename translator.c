#include <stdio.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define OFFSET_MASK 0x0FFF // lower 12 bits
#define SHIFT_AMOUNT 12    // upper 4 bits

// Mock Page Table
int page_table[16] = {5, 2, 8, -1, 12, 1, 9, 3, -1, 7, 4, 6, 10, 15, 11, 14};

uint32_t translate_address(uint16_t virtual_address)
{
    // Extract page number (upper 4 bits)
    uint8_t page_number = virtual_address >> SHIFT_AMOUNT;

    // Extract offset (lower 12 bits)
    uint16_t offset = virtual_address & OFFSET_MASK;

    // Look up physical frame
    int frame = page_table[page_number];

    // Page fault check
    if (frame == -1)
    {
        printf("Page Fault!\n");
        return 0;
    }

    // Build physical address
    uint32_t physical_address = (frame << SHIFT_AMOUNT) | offset;

    return physical_address;
}

int main()
{
    uint16_t va1 = 0x1A2B; // Page 1, Offset 0xA2B
    uint16_t va2 = 0x3004; // Page 3, Offset 0x004 (Should fault)

    printf("VA: 0x%04X -> PA: 0x%05X\n", va1, translate_address(va1));
    printf("VA: 0x%04X -> PA: 0x%05X\n", va2, translate_address(va2));

    return 0;
}