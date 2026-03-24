#pragma once
#include "define.h"
// use static inine for pt_init or pt_allocate?
page_table_t pt_init();
void pt_allocate(page_table_entry_t entry, page_table_t* table);
void pt_free(page_table_t* table);
