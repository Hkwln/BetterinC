#include "table.h"

#include "define.h"
// first of all we fixed the allocation in the stack, we don't use the heap!
page_table_t pt_init()
{
    page_table_t table;
    table.num_pages = NUM_PAGES;
    table.num_frames = NUM_FRAMES;
    table.frames_used = 0;
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
    // FIXME: fix
    // what happens if you allocate the frame two times?
    table->entries[table->frames_used].is_valid = 1;
    table->entries[table->frames_used].frame_number = frame_number;
    table->entries[table->frames_used].protection = protection;
    table->frames_used++;
}
// entry löschen
void unmap_page(uint32_t virt_page_number, page_table_t* table)
{
    if (table->entries[virt_page_number].is_valid) {
        table->entries[virt_page_number].is_valid = 0;
        table->entries[virt_page_number].protection = 0;
        table->frames_used--;
    }
}
void pt_free(page_table_t* table)
{
    for (int i = 0; i < NUM_PAGES; i++) {
        table->entries[i].frame_number = 0;
        table->entries[i].is_valid = 0;
        table->entries[i].protection = 0;
    }
}
