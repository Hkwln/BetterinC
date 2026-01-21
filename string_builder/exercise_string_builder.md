# Coding Exercise: Dynamic String Builder in C

**Difficulty:** Intermediate  
**Topic:** Dynamic memory + String manipulation + Buffer management  
**Estimated Time:** 2-3 hours

---

## Background

You will implement a **dynamic string builder** that efficiently constructs strings without repeated reallocations. Think of it like Java's `StringBuilder` or C++'s `std::stringstream`. This is a practical data structure used in parsers, serializers, and text processing.

**Why is this useful?**  
Concatenating strings with `strcat()` in a loop causes O(n²) performance because each concatenation must copy all previous characters. A string builder uses a growable buffer with amortized O(1) append operations.

---

## Part 1: Data Structure (15 minutes)

Design a struct to represent the string builder:

```c
typedef struct {
    char *buffer;      // Dynamic character buffer
    size_t length;     // Current string length (excluding null terminator)
    size_t capacity;   // Total allocated capacity
} StringBuilder;
```

**Key concepts:**
- `length` tracks actual string size
- `capacity` is always ≥ length + 1 (for null terminator)
- When buffer is full, grow it (similar to dynamic arrays)

**Growth strategy:** When capacity is exceeded, double the capacity (like `std::vector`). This gives amortized O(1) appends.

---

## Part 2: Core Functions (45 minutes)

### Function 1: Create StringBuilder

```c
StringBuilder* sb_create(size_t initial_capacity);
```

- Allocate struct
- Allocate buffer with `initial_capacity` bytes
- Initialize `length = 0`, set `buffer[0] = '\0'`
- If `initial_capacity == 0`, use a default (e.g., 16)

### Function 2: Destroy StringBuilder

```c
void sb_destroy(StringBuilder *sb);
```

- Free buffer, then struct
- Remember the correct order!

### Function 3: Append String

```c
bool sb_append(StringBuilder *sb, const char *str);
```

- Calculate new length: `new_length = sb->length + strlen(str)`
- If `new_length + 1 > sb->capacity`, grow the buffer
- Use `memcpy()` to copy `str` to `sb->buffer + sb->length`
- Update `length` and add null terminator
- Return `true` on success, `false` on allocation failure

**Growth logic:**
```c
while (sb->capacity <= new_length) {
    sb->capacity *= 2;  // Double the capacity
}
sb->buffer = realloc(sb->buffer, sb->capacity);
```

### Function 4: Append Single Character

```c
bool sb_append_char(StringBuilder *sb, char c);
```

- More efficient than appending a 1-character string
- Grow if needed (only 1 byte more)
- Set character and null terminator

### Function 5: Get String

```c
const char* sb_get_string(StringBuilder *sb);
```

- Return pointer to internal buffer (read-only!)
- User should not modify or free this pointer

### Function 6: Clear StringBuilder

```c
void sb_clear(StringBuilder *sb);
```

- Reset `length = 0`
- Set `buffer[0] = '\0'`
- Keep the allocated capacity (no reallocation)

---

## Part 3: Advanced Functions (40 minutes)

### Function 7: Append Formatted (printf-style)

```c
bool sb_append_format(StringBuilder *sb, const char *format, ...);
```

- Use `vsnprintf()` to format into buffer
- First call `vsnprintf()` with NULL to get required size
- Grow buffer if needed
- Call `vsnprintf()` again to actually write

**Hint:** Look up `va_list`, `va_start`, `va_end` for variadic functions.

### Function 8: Take Ownership of String

```c
char* sb_take_string(StringBuilder *sb);
```

- Transfer ownership of buffer to caller
- Caller must `free()` the returned string
- Reset StringBuilder to empty state (allocate new buffer)

### Function 9: Insert String at Position

```c
bool sb_insert(StringBuilder *sb, size_t pos, const char *str);
```

- Insert `str` at index `pos`
- Shift existing characters to the right
- Grow buffer if needed
- Handle edge cases: `pos > length`, `pos == 0`, `pos == length`

---

## Part 4: Utility Functions (30 minutes)

### Function 10: Get Current Length

```c
size_t sb_length(StringBuilder *sb);
```

### Function 11: Get Current Capacity

```c
size_t sb_capacity(StringBuilder *sb);
```

### Function 12: Reserve Capacity

```c
bool sb_reserve(StringBuilder *sb, size_t new_capacity);
```

- Ensure capacity is at least `new_capacity`
- Useful to avoid multiple reallocations when final size is known

### Function 13: Shrink to Fit

```c
void sb_shrink_to_fit(StringBuilder *sb);
```

- Reallocate buffer to exactly `length + 1` bytes
- Reduces memory waste

---

## Part 5: Test Program (30 minutes)

Write a `main()` function that demonstrates:

1. **Basic append:** Build "Hello, World!" from pieces
2. **Formatted append:** Add numbers and mixed types
3. **Character append:** Build string character by character
4. **Performance test:** Append 10,000 small strings (show it's fast)
5. **Insert test:** Insert into middle of string
6. **Memory test:** Verify no leaks with Valgrind

**Example test:**
```c
StringBuilder *sb = sb_create(0);
sb_append(sb, "Hello");
sb_append(sb, ", ");
sb_append(sb, "World");
sb_append_char(sb, '!');
printf("%s\n", sb_get_string(sb));  // "Hello, World!"
printf("Length: %zu, Capacity: %zu\n", sb_length(sb), sb_capacity(sb));
sb_destroy(sb);
```

---

## Extension Ideas (Optional)

1. **Append substring:** `sb_append_range(sb, str, start, end)`
2. **Find and replace:** Replace all occurrences of substring
3. **Trim whitespace:** Remove leading/trailing spaces
4. **Reverse:** Reverse the string in-place
5. **Join array:** Build string from array of strings with delimiter
6. **Copy constructor:** Create copy of another StringBuilder
7. **Compare:** Check if two StringBuilders are equal
8. **To uppercase/lowercase:** Transform characters
9. **Buffer pooling:** Reuse old buffers instead of freeing

---

## What You'll Learn

✓ Dynamic memory management with `realloc()`  
✓ Amortized complexity analysis  
✓ Efficient string operations  
✓ Variadic functions (`printf`-style formatting)  
✓ Buffer management strategies  
✓ Performance vs. memory trade-offs  
✓ API design (ownership semantics)

---

## Hints

- **Start simple:** Get create/destroy/append working first
- **Test growth:** Force multiple reallocations with small initial capacity
- **Draw it out:** Sketch buffer state after each operation
- **Watch null terminators:** Every operation must maintain `buffer[length] == '\0'`
- **Check return values:** `realloc()` can fail (return NULL)
- **Use assert:** Add `assert(sb != NULL)` at function start during development
- **Print capacity:** Debug growth by printing capacity after each append

---

## Common Pitfalls

⚠️ **Off-by-one errors:** Remember capacity must fit `length + 1` for null terminator  
⚠️ **Forgetting realloc can move buffer:** Don't keep old pointers to buffer contents  
⚠️ **Not checking realloc failure:** Always check if `realloc()` returns NULL  
⚠️ **String operations after take_string:** Buffer is gone after ownership transfer  

---

## Submission Files

- `string_builder.h` - Header with struct and function declarations
- `string_builder.c` - Implementation
- `main.c` - Test/demo program
- `Makefile` - Build configuration

---

**This exercise is more practical than circular buffer and combines concepts from your arena allocator (growth strategy) and hash table (dynamic resizing). It's used in real parsers, JSON serializers, and text generators!**
