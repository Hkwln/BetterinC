#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define PAGE_SIZE 4096  // should be 4Kb
#define NUM_PAGES 20
#define NUM_FRAMES 5
#define READ (1 << 0)
#define WRITE (1 << 1)
#define EXECUTE (1 << 2)

typedef struct page_table_entry_t {
    // physical_addr = frame_number * page_size + offset
    uint32_t frame_number;  // where in pyhsical memory
    uint8_t is_valid;
    uint8_t protection;
} page_table_entry_t;

typedef struct {
    page_table_entry_t entries[NUM_PAGES];
    size_t num_frames;
    size_t num_pages;
    size_t frames_used;
} page_table_t;

static inline uint32_t virt_to_phys(uint32_t virt_addr, page_table_t* table)
{
    // vfn = virtual frame number
    uint32_t vfn = virt_addr / PAGE_SIZE;
    // TODO: bounds checking
    uint32_t offset = virt_addr % PAGE_SIZE;
    // TODO: errror handling
    if (table->entries[vfn].is_valid == 0) {
        printf("error, this entrie is invalid\n");
        return 0;
    }
    uint32_t frame_number = table->entries[vfn].frame_number;
    return frame_number * PAGE_SIZE + offset;
}
