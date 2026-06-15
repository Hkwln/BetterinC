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
    uint8_t frames_used[NUM_FRAMES];  // bit map tracking which physical frames
                                      // are in use
} page_table_t;

//* Szenario virt addr = 100;
static inline uint32_t virt_to_phys(uint32_t virt_addr, page_table_t* table)
{
    // vfn = virtual frame number
    uint32_t vfn = virt_addr / PAGE_SIZE;  //* vfn = 0 //ATTENTION: is this
                                           // righ? vfn is almost always under
                                           // 1; should i rather use a float?
    // TODO: bounds checking should i use errno? and which error code should i
    // use?
    if (vfn >= table->num_pages) {
        printf("error, this entry is invalid\n");
        return UINT32_MAX;
    }
    uint32_t offset = virt_addr % PAGE_SIZE;  //* offset = 100;
    // TODO: errror handling
    if (table->entries[vfn].is_valid == 0) {
        printf("error, this entrie is invalid\n");
        return UINT32_MAX;
    }
    uint32_t frame_number =
        table->entries[vfn].frame_number;  //* frame_number = table
                                           // entries[0].frame_number = 0;

    return frame_number * PAGE_SIZE + offset;  //* 0 * 0 + 100 = 100;
}
