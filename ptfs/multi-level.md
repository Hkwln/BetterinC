# Spoiler: Writen by ai; only for me to understand.

# Multi-Level Page Tables — Design Guide

## Why Multi-Level?

Your current page table is a **flat array** of 20 PTEs. For small address spaces this works,
but real systems have huge address spaces (e.g. 2^32 = 4 GB with 4 KB pages = 1 million PTEs).
A flat array of 1 million entries wastes enormous memory — most pages are never mapped.

A **multi-level** table solves this by using a tree structure:

- Only allocate sub-tables for virtual pages that are actually mapped
- Unmapped ranges require zero memory (the top-level entry is just "not present")

## How Virtual Address Bits Work

A virtual address is split into parts at each level:

```
Single-level (what you have now):
  [ VPN (bits 12–31) ][ offset (bits 0–11) ]
       20 bits             12 bits

Two-level (next step):
  [  PD index (bits 22–31) ][  PT index (bits 12–21) ][ offset (bits 0–11) ]
         10 bits                   10 bits                 12 bits
```

- **Offset** (12 bits): byte within the 4 KB page — same as before
- **PD index**: which entry in the Page Directory
- **PT index**: which entry in the Page Table
- Each level's entry points to the next level's table (or to a physical frame)

## Naming Convention

| Level      | Name                | What it holds               |
| ---------- | ------------------- | --------------------------- |
| Level 1    | Page Directory (PD) | Pointers to Page Tables     |
| Level 2    | Page Table (PT)     | Pointers to Physical Frames |
| (Level 3+) | Deeper levels       | Same pattern repeated       |

## Walking the Table

To translate a virtual address with a 2-level table:

```c
// Step 1: Extract the indices from the virtual address
uint32_t pd_index = (virt_addr >> 22) & 0x3FF;   // top 10 bits
uint32_t pt_index = (virt_addr >> 12) & 0x3FF;   // middle 10 bits
uint32_t offset   =  virt_addr        & 0xFFF;   // bottom 12 bits

// Step 2: Walk Level 1 (Page Directory)
page_directory_entry_t pde = page_dir[pd_index];
if (!pde.present) {
    // page fault — no page table allocated for this range
    return error;
}

// Step 3: Walk Level 2 (Page Table)
page_table_t* pt = pde.pt_pointer;   // pointer or frame number
page_table_entry_t pte = pt->entries[pt_index];
if (!pte.present) {
    // page fault — this specific page isn't mapped
    return error;
}

// Step 4: Compute physical address
return pte.frame_number * PAGE_SIZE + offset;
```

## Data Structures

```c
#define PAGE_SIZE 4096

// Level 2: Page Table Entry (same as what you already have)
typedef struct {
    uint32_t frame_number;
    uint8_t  present;      // 1 = mapped to a physical frame
    uint8_t  protection;   // READ/WRITE/EXECUTE
} pte_t;

// Level 1: Page Directory Entry
// Points to a Level 2 Page Table (or is empty)
typedef struct {
    uint32_t pt_frame;     // frame number of the page table itself
    uint8_t  present;      // 1 = page table exists for this range
} pde_t;

// A single page table contains PT_ENTRIES entries
#define PT_ENTRIES 1024    // 2^10 entries per level

typedef struct {
    pte_t entries[PT_ENTRIES];
} page_table_t;

// The top-level page directory
typedef struct {
    pde_t entries[PT_ENTRIES];   // 1024 PDEs
} page_directory_t;
```

## Key Operations

### 1. Map a page (`map_page(virt_addr, frame_number, protection, &page_dir)`)

```c
void map_page(uint32_t virt_addr, uint32_t frame_number,
              uint8_t prot, page_directory_t* page_dir)
{
    uint32_t pd_idx = (virt_addr >> 22) & 0x3FF;
    uint32_t pt_idx = (virt_addr >> 12) & 0x3FF;

    // If the page directory entry is not present, allocate a new page table
    if (!page_dir->entries[pd_idx].present) {
        // Allocate a physical frame to hold the page table
        uint32_t pt_frame = allocate_frame();
        page_dir->entries[pd_idx].pt_frame = pt_frame;
        page_dir->entries[pd_idx].present = 1;
        // Zero out the new page table (all entries start not-present)
        page_table_t* pt = get_frame_pointer(pt_frame);
        memset(pt, 0, sizeof(page_table_t));
    }

    // Get or compute the pointer to the page table
    page_table_t* pt = /* pointer to the page table at pt_frame */;

    // Set the PTE
    pt->entries[pt_idx].frame_number = frame_number;
    pt->entries[pt_idx].present = 1;
    pt->entries[pt_idx].protection = prot;
}
```

### 2. Translate (`virt_to_phys(virt_addr, &page_dir)`)

```c
uint32_t virt_to_phys(uint32_t virt_addr, page_directory_t* page_dir)
{
    uint32_t pd_idx = (virt_addr >> 22) & 0x3FF;
    uint32_t pt_idx = (virt_addr >> 12) & 0x3FF;
    uint32_t offset =  virt_addr        & 0xFFF;

    if (!page_dir->entries[pd_idx].present)
        return UINT32_MAX;   // page fault

    // Get the page table
    page_table_t* pt = ...;
    if (!pt->entries[pt_idx].present)
        return UINT32_MAX;   // page fault

    return pt->entries[pt_idx].frame_number * PAGE_SIZE + offset;
}
```

## Memory Overhead Comparison

| Scheme         | 1 page mapped      | 1024 pages mapped      | Full address space |
| -------------- | ------------------ | ---------------------- | ------------------ |
| Flat (1-level) | 4 MB               | 4 MB                   | 4 MB               |
| 2-level        | 4 KB + 4 KB = 8 KB | 4 KB + 4 KB × 1 = 8 KB | 4 KB + 4 MB ≈ 4 MB |

- **Flat** always allocates space for every possible page (wasteful for sparse use)
- **2-level** only allocates a page table when at least one page in that 4 MB range is mapped

## Extending to 3+ Levels

Same pattern, just more index splits in the virtual address:

```
3-level (e.g. x86 PAE):
  [ PDP idx ][ PD idx ][ PT idx ][ offset ]
     2 bits    9 bits    9 bits    12 bits

4-level (x86-64):
  [ PML4 ][ PDP ][ PD ][ PT ][ offset ]
     9      9      9     9      12
```

Each level adds another indirection. The walk always follows the same pattern:
extract index → check present → follow pointer → repeat.

## How to Test (No Kernel Needed)

You can simulate everything in userspace:

```c
// Instead of real physical frames, use a large byte array
uint8_t physical_memory[NUM_FRAMES * PAGE_SIZE];

// "Allocating a frame" = choosing an index into this array
// "Getting a pointer to a frame" = &physical_memory[frame * PAGE_SIZE]

uint32_t allocate_frame() {
    // pick any free frame index
}

// A "pointer to a page table" is just a cast:
page_table_t* pt = (page_table_t*)&physical_memory[pt_frame * PAGE_SIZE];
```

This is exactly what you're already doing with `write_mem` and `read_mem` — just extend
the same technique to the page directory and page tables.

## Common Pitfalls

1. **Off-by-one in bit shifts.** Double-check your mask values:
   - `(virt_addr >> 22) & 0x3FF` = bits 22–31 (10 bits)
   - `(virt_addr >> 12) & 0x3FF` = bits 12–21 (10 bits)
   - `virt_addr & 0xFFF` = bits 0–11 (12 bits)

2. **Forgetting to zero a new page table.** A freshly allocated frame has undefined
   content — you must zero it so all PTEs start as `present=0`.

3. **Leaking page tables.** When the last mapping in a page table is removed, you
   should free the page table's frame back to the allocator.

4. **Circular pointers.** Never let a PDE point to a frame that contains the page
   directory itself — this creates infinite loops in the walk.
