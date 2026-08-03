# Page Table Implementation - Step by Step TODO

copilot --resume=4272e2ca-2c03-4a27-a7b6-bfaf8285b461

- rewrite so it allocates REAL pages with mmap

## Phase 1: Research & Understanding

- [ ] Read Wikipedia article on page tables
- [ ] Read Art of Unix Programming Chapter 1
- [ ] Read OSTEP Chapters 13-16 on virtual memory
- [ ] Review APUE Chapter 7 and 14 (optional)
- [ ] Document key concepts: virtual addresses, physical addresses, page size, page table entries

## Phase 2: Design Simple Page Table Structure

- [x] Define page size (typically 4KB)
- [x] Define virtual address space size
- [x] Define physical memory size
- [x] Design page table entry (PTE) structure:
  - [x] Physical frame number
  - [x] Valid/invalid bit
  - [x] Protection bits (read/write/execute)
  - [x] Present bit
- [x] Document address translation formula: `physical_addr = frame_number * page_size + offset`

## Phase 3: Basic Data Structures

- [x] Create `page_table_entry` struct with necessary fields
- [x] Create `page_table` struct to hold array of PTEs
- [x] Implement page table initialization function
- [x] Implement function to allocate physical frames

## Phase 4: Core Translation Functions

- [x] Implement `virtual_to_physical()` address translation
  - [x] Extract page number from virtual address
  - [x] Extract offset from virtual address
  - [x] Look up PTE in page table
  - [x] Check valid bit
  - [x] Compute physical address
- [x] Implement `map_page()` to map virtual page to physical frame
- [x] Implement `unmap_page()` to remove mapping

## Phase 5: Memory Management

- [x] Create free frame list/bitmap
- [x] Implement `allocate_frame()` to get free physical frame
- [x] Implement `free_frame()` to return frame to free list
- [x] Handle page faults (when valid bit is 0)

## Phase 6: Testing

- [x] Test basic address translation
- [x] Test mapping and unmapping pages
- [x] Test with multiple virtual addresses mapping to different frames
- [ ] Test boundary conditions (invalid addresses, full memory)
- [ ] Test page fault handling

## Phase 7: Simple Assumptions (Initial Version)

- [ ] Address spaces placed contiguously
- [ ] sizeof(address_space) < sizeof(physical_memory)
- [ ] All address spaces are same size
- [ ] Use simple formula: `physical_addr = virtual_addr + base`

## Phase 8: Enhancements (Optional)

- [ ] Add multi-level page tables (if needed)
- [ ] Implement TLB (Translation Lookaside Buffer) simulation
- [ ] Add page replacement policies (FIFO, LRU)
- [ ] Add dirty bit for tracking modifications
- [ ] Implement shared pages between processes

## Notes

- Start with simplest possible implementation
- Test incrementally after each phase
- Reference existing memory pool implementation for ideas
- Keep physical memory simulation simple (malloc'd array)
