#include "table.h"

#include "define.h"

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
/* TODO: correct this wrong implementation*/
void pt_allocate(page_table_entry_t entry, page_table_t* table)
{
    table->entries[table->frames_used] = entry;
    table->num_pages++;
    table->frames_used++;
    table->num_pages++;
}
void pt_free(page_table_t* table)
{
    // do something
}
