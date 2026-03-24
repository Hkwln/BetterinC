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
