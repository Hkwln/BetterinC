#pragma once
#include "define.h"

// use static inine for pt_init or pt_allocate?
page_table_t pt_init();
void map_page(uint32_t virt_addr, uint32_t frame_number, uint8_t protection,
              page_table_t* table);
void unmap_page(uint32_t virt_page_number, page_table_t* table);
void pt_free(page_table_t* table);
int allocate_frame(page_table_t* table);
void free_frame(page_table_t* table, uint8_t frame);
