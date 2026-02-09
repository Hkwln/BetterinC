# Optimization TODO: Active Cell Tracking

## The Problem

Currently, `loop()` checks EVERY cell in the grid (width × height) every generation.
For a 40×40 grid = 1,600 cells checked per generation.
But in Game of Life, most cells are dead and stay dead!

## The Hash Set Strategy Explained

### Core Concept

Only track and check cells that MIGHT change:

- All currently alive cells
- All cells neighboring alive cells (the "frontier")

Example: If only 50 cells are alive, you only need to check ~450 cells (50 alive + ~400 neighbors) instead of 1,600!

### Why "Hash Set"?

You need two capabilities:

1. **Fast lookup**: "Is cell (x,y) active?" → O(1) time
2. **Iteration**: Loop through all active cells → O(active_count) time

A hash set (or bitmap as alternative) provides both.

## Implementation Steps

### Step 1: Update Data Structures

- [ ] Modify `Active` struct in `data.h` to store:
  - A list/array of cell coordinates (for iteration)
  - A fast lookup structure (bitmap or hash)
  - Count of active cells
- [ ] Think about: How will you map (x,y) coordinates to array indices?

### Step 2: Implement Active Cell Management

- [ ] Write function to add a cell to active set (avoid duplicates!)
- [ ] Write function to check if a cell is in active set
- [ ] Write function to clear/reset the active set
- [ ] Fix the broken `set_active()` logic in `data.c`

### Step 3: Build Active Set Each Generation

- [ ] Before calling `loop()`, build the active set:
  - Scan all currently alive cells
  - For each alive cell, mark it + its 8 neighbors as active
- [ ] Think about: Should you use the old bitmap or new bitmap?

### Step 4: Modify the Loop Function

- [ ] Change `loop()` to iterate over active cells only, not all cells
- [ ] Consider: What happens to cells that become active mid-generation?
- [ ] Handle edge cases: cells at borders, empty grids

### Step 5: Test & Benchmark

- [ ] Run with small grid, verify correctness
- [ ] Compare performance: old vs new (use the timer!)
- [ ] Test edge cases: all dead, all alive, oscillators, gliders

## Alternative Approaches

### Approach A: Bitmap (Simple)

Use a uint8_t array where `active_bitmap[y*width + x]` = 1 if active

- Pro: Very simple, O(1) lookup
- Con: Still need to iterate or maintain separate list

### Approach B: Coordinate Array (Medium)

Store only (x,y) pairs in a dynamic array

- Pro: Direct iteration
- Con: Need to prevent duplicates, harder lookup

### Approach C: Two Arrays (Recommended)

Both a bitmap AND a coordinate list

- Pro: Fast lookup + fast iteration
- Con: More memory, must keep in sync

## Key Questions to Answer

1. How do you prevent adding duplicate cells to your active set?
2. When do you build the active set - before or after computing next generation?
3. Should you use one active set or swap between two (like the bitmaps)?
4. How do you handle the coordinate→index conversion efficiently?

## Hints (Don't Read Unless Stuck!)

<details>
<summary>Hint 1: Index Mapping</summary>
Flat index = y * width + x
This maps 2D coordinates to 1D array
</details>

<details>
<summary>Hint 2: Double Buffering</summary>
You might need two active sets, just like live/live2/live3
One for "currently active" and one for "next generation active"
</details>

<details>
<summary>Hint 3: Duplicate Prevention</summary>
Before adding to coordinate list, check the bitmap first
If bitmap[index] is already 1, don't add to list again
</details>

## Expected Performance Gain

- Sparse patterns (few alive cells): **10-100x faster**
- Dense patterns (many alive cells): **1-2x faster**
- Worst case (all alive + neighbors): **Same speed** as before

Good luck! This is a classic optimization pattern used in many simulation engines.
