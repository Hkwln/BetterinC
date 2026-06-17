#include "table.h"

#include <stdint.h>

#include "define.h"

// linear scan of bitmap, returns a free frame number
int allocate_frame(page_table_t* table)
{
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (table->frames_used[i] == 0) {
            table->frames_used[i] = 1;
            return i;
        }
    }
    return -1;
}
// first of all we fixed the allocation in the stack, we don't use the heap!
page_table_t pt_init()
{
    page_table_t table;
    table.num_pages = NUM_PAGES;
    table.num_frames = NUM_FRAMES;
    // init frame tracker:
    for (int i = 0; i < NUM_FRAMES; i++) {
        table.frames_used[i] = 0;
    }
    for (int i = 0; i < NUM_PAGES; i++) {
        table.entries[i].frame_number = 0;
        table.entries[i].is_valid = 0;
        table.entries[i].protection = 0;
    }
    return table;
}

// entrie hinzufügen
void map_page(uint32_t virt_addr, uint32_t frame_number, uint8_t protection,
              page_table_t* table)
{
    uint32_t vpn = virt_addr / PAGE_SIZE;
    // TODO: bounds check on vpn < table->num_pages
    table->entries[vpn].is_valid = 1;
    table->entries[vpn].frame_number = frame_number;
    table->entries[vpn].protection = protection;
}

void write_mem(uint32_t virt_addr, uint8_t value, page_table_t* table,
               uint8_t* phys_mem)
{
    // TODO:check if page is already mapped
    map_page(virt_addr, allocate_frame(table), (WRITE | EXECUTE), table);
    phys_mem[virt_to_phys(virt_addr, table)] = value;
}
uint8_t read_mem(uint32_t virt_addr, page_table_t* table, uint8_t* phys_mem)
{
    return phys_mem[virt_to_phys(virt_addr, table)];
}
// marks a frame as free in the bitmap
void free_frame(page_table_t* table, uint8_t frame)
{
    table->frames_used[frame] = 0;
}
// entry löschen
void unmap_page(uint32_t virt_page_number, page_table_t* table)
{
    if (table->entries[virt_page_number].is_valid) {
        table->entries[virt_page_number].is_valid = 0;
        table->entries[virt_page_number].protection = 0;
        int fn = table->entries[virt_page_number].frame_number;
        if (fn < NUM_FRAMES) free_frame(table, fn);
    }
}
void pt_free(page_table_t* table)
{
    // TODO: add bounds check
    for (int i = 0; i < NUM_PAGES; i++) {
        table->entries[i].frame_number = 0;
        table->entries[i].is_valid = 0;
        table->entries[i].protection = 0;
    }
    for (int i = 0; i < NUM_FRAMES; i++) {
        table->frames_used[i] = 0;
    }
}
