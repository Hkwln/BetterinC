# Coding Exercise: Bitmap Image Manipulation in C

**Difficulty:** Intermediate  
**Topic:** Bit operations + Data structures + File I/O  
**Estimated Time:** 2-3 hours

---

## Background

You will implement a simple **1-bit bitmap image** library. A 1-bit bitmap stores each pixel as a single bit (0 = black, 1 = white). This exercise combines:

- Bit manipulation (setting/getting individual bits)
- Memory management (malloc/free)
- Data structures (struct with dynamic arrays)
- File I/O (reading/writing binary data)

This is similar to what image formats like BMP or PBM do internally!

---

## Part 1: Data Structure (15 minutes)

Create a struct to represent a bitmap image:

```c
typedef struct {
    uint32_t width;   // Image width in pixels
    uint32_t height;  // Image height in pixels
    uint8_t* data;    // Bit array (1 bit per pixel)
} Bitmap;
```

**Important:** Each pixel is 1 bit, but you can only allocate **bytes**. So for a 10×10 image (100 pixels), you need `⌈100/8⌉ = 13` bytes.

Formula: `bytes_needed = (width * height + 7) / 8`

---

## Part 2: Core Functions (45 minutes)

### Function 1: Create Bitmap

```c
Bitmap* bitmap_create(uint32_t width, uint32_t height);
```

- Allocate memory for the struct
- Calculate how many bytes needed for `data`
- Use `calloc()` to initialize all bits to 0 (black)
- Return pointer to the bitmap

### Function 2: Destroy Bitmap

```c
void bitmap_destroy(Bitmap* bmp);
```

- Free the data array
- Free the struct
- Avoid memory leaks!

### Function 3: Set Pixel

```c
void bitmap_set_pixel(Bitmap* bmp, uint32_t x, uint32_t y, bool white);
```

- Calculate bit index: `bit_index = y * width + x`
- Calculate which byte: `byte_index = bit_index / 8`
- Calculate which bit in that byte: `bit_offset = bit_index % 8`
- Use bit operations to set or clear the bit

**Hint:** Use your cheat sheet! Remember `x | (1 << n)` and `x & ~(1 << n)`.

### Function 4: Get Pixel

```c
bool bitmap_get_pixel(Bitmap* bmp, uint32_t x, uint32_t y);
```

- Same calculation as set_pixel
- Extract the bit and return true (white) or false (black)

---

## Part 3: Drawing Functions (30 minutes)

### Function 5: Fill Rectangle

```c
void bitmap_fill_rect(Bitmap* bmp, uint32_t x, uint32_t y,
                      uint32_t width, uint32_t height, bool white);
```

- Draw a filled rectangle starting at (x, y)
- Use nested loops and `bitmap_set_pixel()`

### Function 6: Draw Horizontal Line

```c
void bitmap_draw_hline(Bitmap* bmp, uint32_t x, uint32_t y,
                       uint32_t length, bool white);
```

- More efficient than setting pixels one by one
- **Challenge:** Try to set multiple bits at once if they're in the same byte!

---

## Part 4: File I/O (45 minutes)

### Function 7: Save to ASCII File

```c
void bitmap_save_ascii(Bitmap* bmp, const char* filename);
```

- Save the bitmap as a text file
- Use `#` for white pixels, `.` for black pixels
- Each line = one row of pixels
- Example output for 5×3 bitmap:

```
..#..
#...#
..#..
```

This helps you visualize and debug your bitmap!

### Function 8: Save to Binary File (BONUS)

```c
void bitmap_save_pbm(Bitmap* bmp, const char* filename);
```

- Save in Netpbm PBM format (real image format!)
- Header: `P4\n<width> <height>\n`
- Then write raw `data` bytes
- You can open it with image viewers!

---

## Part 5: Test Program (30 minutes)

Write a `main()` function that:

1. Creates a 32×32 bitmap
2. Draws a white border (rectangle around edge)
3. Draws a white cross in the middle
4. Saves it as ASCII file
5. (BONUS) Saves it as PBM file
6. Properly destroys the bitmap

---

## Testing Checklist

- [ ] Can create bitmaps of different sizes
- [ ] Setting/getting pixels works correctly
- [ ] No segmentation faults
- [ ] No memory leaks (test with `valgrind ./your_program`)
- [ ] ASCII output looks correct when printed
- [ ] Edge cases: pixel at (0,0), pixel at (width-1, height-1)
- [ ] Edge cases: 1×1 bitmap, 7×1 bitmap (less than one byte)

---

## Extension Ideas (Optional)

If you finish early or want more challenge:

1. **Invert bitmap**: Flip all bits (black↔white)
2. **Copy bitmap**: Deep copy function
3. **Draw circle**: Use Bresenham's circle algorithm
4. **Flip horizontally/vertically**
5. **Load PBM file**: Read bitmap from file
6. **Bitwise operations on images**: AND/OR/XOR two bitmaps

---

## What You'll Learn

✓ Bit-level memory layout  
✓ Working with packed data structures  
✓ Index calculations (2D → 1D mapping)  
✓ Binary file I/O  
✓ Practical use of bit operations  
✓ Memory management best practices

---

## Hints

- Start simple: Get `bitmap_create()` and `bitmap_set_pixel()` working first
- Print bytes in binary to debug: `printf("%08b", byte)`
- Draw on paper: Sketch a 3×3 bitmap and calculate byte/bit indices manually
- Test with small bitmaps (3×3 or 8×8) before going to 32×32
- Remember: Bits are usually numbered **right to left** (bit 0 = rightmost)

---

## Submission

When finished, you should have:

- `bitmap.h` - Header file with struct and function declarations
- `bitmap.c` - Implementation of all functions
- `main.c` - Test program
- `Makefile` - To compile everything

**Good luck! This is a real-world applicable exercise that combines multiple C concepts.**
