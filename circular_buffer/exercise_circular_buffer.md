# Coding Exercise: Circular Buffer (Ring Buffer) in C

**Difficulty:** Intermediate  
**Topic:** Data structures + Modulo arithmetic + File I/O + Producer-Consumer pattern  
**Estimated Time:** 2-3 hours

---

## Background

A **circular buffer** (ring buffer) is a fixed-size buffer that wraps around. When you reach the end, you start writing at the beginning again. It's heavily used in:

- **Audio/video streaming** (FFmpeg, OpenIPC camera streams)
- **Serial communication** (UART buffers)
- **Kernel logging** (dmesg ring buffer)
- **Producer-consumer problems** (one thread writes, another reads)

**Key property:** Constant-time O(1) insert/remove, no reallocation needed!

---

## Part 1: Core Structure (15 minutes)

```c
typedef struct {
    uint8_t* buffer;    // The data storage
    size_t capacity;    // Total size in bytes
    size_t read_pos;    // Where to read next
    size_t write_pos;   // Where to write next
    size_t count;       // Number of bytes currently stored
} CircularBuffer;
```

**Visualization:**

```
[_ _ _ D A T A _ _ _]
       ^       ^
     read    write

capacity = 10
count = 4 (DATA)
read_pos = 3
write_pos = 7
```

**Key insight:** Use `count` to distinguish empty vs full:

- Empty: `count == 0`
- Full: `count == capacity`
- Available space: `capacity - count`

---

## Part 2: Basic Functions (45 minutes)

### Function 1: Create Buffer

```c
CircularBuffer* cb_create(size_t capacity);
```

- Allocate struct and data buffer
- Initialize all positions to 0
- Return pointer

### Function 2: Destroy Buffer

```c
void cb_destroy(CircularBuffer* cb);
```

- Free buffer and struct
- Check for NULL before freeing

### Function 3: Write Single Byte

```c
bool cb_write(CircularBuffer* cb, uint8_t byte);
```

- Check if full:
- Write to buffer:
- Advance write position:
- Increment count:
- Return true

**The modulo `%` makes it circular!**

### Function 4: Read Single Byte

```c
bool cb_read(CircularBuffer* cb, uint8_t* byte);
```

- Check if empty:
- Read from buffer:
- Advance read position:
- Decrement count:
- Return true

---

## Part 3: Bulk Operations (30 minutes)

### Function 5: Write Multiple Bytes

```c
size_t cb_write_bulk(CircularBuffer* cb, const uint8_t* data, size_t size);
```

**Algorithm:**

1. Calculate available space:
2. Limit write:
3. Split into two parts if wrap-around occurs:

   ```
   size_t first_chunk = cb->capacity - cb->write_pos;
   if (to_write <= first_chunk) {
       // No wrap: single memcpy
       memcpy(cb->buffer + cb->write_pos, data, to_write);
   } else {
       // Wrap: two memcpy calls
       memcpy(cb->buffer + cb->write_pos, data, first_chunk);
       memcpy(cb->buffer, data + first_chunk, to_write - first_chunk);
   }
   ```

4. Update positions and count
5. Return bytes written

### Function 6: Read Multiple Bytes

```c
size_t cb_read_bulk(CircularBuffer* cb, uint8_t* dest, size_t size);
```

- Similar to write_bulk, but reads
- Handle wrap-around the same way
- Return bytes read

**Why bulk operations?** 10-100x faster than byte-by-byte for large data!

---

## Part 4: Utility Functions (20 minutes)

### Function 7: Peek (Read Without Removing)

```c
bool cb_peek(CircularBuffer* cb, uint8_t* byte, size_t offset);
```

- Read byte at `(read_pos + offset) % capacity`
- Don't modify read_pos or count
- Check: `if (offset >= cb->count) return false;`

### Function 8: Available Space

```c
size_t cb_space_available(CircularBuffer* cb);
```

- Return `cb->capacity - cb->count`

### Function 9: Available Data

```c
size_t cb_data_available(CircularBuffer* cb);
```

- Return `cb->count`

### Function 10: Clear Buffer

```c
void cb_clear(CircularBuffer* cb);
```

- Set `read_pos = write_pos = count = 0`
- Don't need to clear actual data!

---

## Part 5: Advanced Feature - Overwrite Mode (25 minutes)

Sometimes you want newest data to overwrite oldest data when full (like dmesg).

### Function 11: Write with Overwrite

```c
void cb_write_overwrite(CircularBuffer* cb, uint8_t byte);
```

```c
if (cb->count >= cb->capacity) {
    // Full: overwrite oldest data
    cb->buffer[cb->write_pos] = byte;
    cb->write_pos = (cb->write_pos + 1) % cb->capacity;
    cb->read_pos = (cb->read_pos + 1) % cb->capacity;  // Skip oldest
    // count stays at capacity
} else {
    // Normal write
    cb->buffer[cb->write_pos] = byte;
    cb->write_pos = (cb->write_pos + 1) % cb->capacity;
    cb->count++;
}
```

---

## Part 6: Test Program (45 minutes)

### Test 1: Basic Write/Read

```c
CircularBuffer* cb = cb_create(10);
cb_write(cb, 'A');
cb_write(cb, 'B');
uint8_t c;
cb_read(cb, &c);  // Should read 'A'
printf("Read: %c\n", c);
```

### Test 2: Fill and Empty

```c
// Fill buffer
for (int i = 0; i < 10; i++) {
    bool ok = cb_write(cb, 'X');
    assert(ok);
}
assert(!cb_write(cb, 'Y'));  // Should fail (full)

// Empty buffer
for (int i = 0; i < 10; i++) {
    uint8_t b;
    assert(cb_read(cb, &b));
}
uint8_t b;
assert(!cb_read(cb, &b));  // Should fail (empty)
```

### Test 3: Wrap-Around

```c
cb_clear(cb);
for (int i = 0; i < 15; i++) {  // More than capacity
    cb_write(cb, '0' + (i % 10));
    uint8_t c;
    if (i >= 5) {  // Start reading after 5 writes
        cb_read(cb, &c);
        printf("%c ", c);
    }
}
```

### Test 4: Bulk Operations

```c
cb_clear(cb);
const char* msg = "Hello";
size_t written = cb_write_bulk(cb, (uint8_t*)msg, 5);
printf("Wrote %zu bytes\n", written);

char buf[10];
size_t read = cb_read_bulk(cb, (uint8_t*)buf, 5);
buf[read] = '\0';
printf("Read: %s\n", buf);
```

### Test 5: Peek

```c
cb_clear(cb);
cb_write(cb, 'X');
cb_write(cb, 'Y');
cb_write(cb, 'Z');

uint8_t b;
cb_peek(cb, &b, 0);  // First byte
printf("Peek[0]: %c\n", b);
cb_peek(cb, &b, 2);  // Third byte
printf("Peek[2]: %c\n", b);
// Nothing should be removed!
assert(cb_data_available(cb) == 3);
```

### Test 6: Overwrite Mode

```c
cb_clear(cb);
for (int i = 0; i < 15; i++) {
    cb_write_overwrite(cb, '0' + i);
}
// Buffer should contain: 5,6,7,8,9,10,11,12,13,14 (10 newest)
```

---

## Testing Checklist

- [ ] Create/destroy works without leaks
- [ ] Write fails when full (normal mode)
- [ ] Read fails when empty
- [ ] Wrap-around works correctly
- [ ] Bulk operations handle wrap-around
- [ ] Peek doesn't modify buffer
- [ ] Overwrite mode keeps newest data
- [ ] Works with capacity=1 (edge case)
- [ ] Works with capacity=1000000 (large buffer)
- [ ] `valgrind` shows no leaks or errors

---

## Extension Ideas (Optional)

1. **Thread-safe version**: Add mutex for producer-consumer threads
2. **Wait/signal**: Block when full/empty (use condition variables)
3. **Read line**: Read until '\n' character
4. **Write string**: `cb_write_string(cb, "Hello")`
5. **Statistics**: Track total bytes written/read, overflows
6. **Save/load**: Write buffer state to file, restore later
7. **Audio example**: Simulate audio streaming (write samples, read for playback)

---

## What You'll Learn

✓ Circular/ring buffer data structure  
✓ Modulo arithmetic for wrapping  
✓ Handling edge cases (empty/full/wrap-around)  
✓ Bulk operations with memcpy  
✓ Producer-consumer pattern basics  
✓ Data structure used in FFmpeg/OpenIPC

---

## Hints

- **Modulo trick:** `% capacity` ensures positions never exceed bounds
- **Count is key:** Easier than tracking "is full" separately
- **Bulk operations:** Always check for wrap-around!
- **Debug visualization:** Print buffer with `[...]` showing read/write positions
- **Common bug:** Forgetting to update all three values (position, count, and the other position in overwrite mode)

**Visualization helper function:**

```c
void cb_print(CircularBuffer* cb) {
    printf("Buffer (capacity=%zu, count=%zu):\n[", cb->capacity, cb->count);
    for (size_t i = 0; i < cb->capacity; i++) {
        if (i == cb->read_pos && i == cb->write_pos) {
            printf("RW");
        } else if (i == cb->read_pos) {
            printf("R");
        } else if (i == cb->write_pos) {
            printf("W");
        }
        printf("%c ", cb->buffer[i]);
    }
    printf("]\n");
}
```

---

## Real-World Usage

**FFmpeg:**

- Audio/video frame buffering between decoder and encoder
- Network packet buffering for streaming

**Linux Kernel:**

- `dmesg` ring buffer (fixed size, overwrites old logs)
- UART receive buffer

**OpenIPC:**

- Video frame queue before encoding
- Network buffer for RTSP streaming

**Embedded Systems:**

- Sensor data buffering
- Command queue for motors/servos

---

---

## Connection to FFmpeg Goal

This is a **fundamental building block** in FFmpeg. Understanding circular buffers helps you:

- Read FFmpeg source code (they're everywhere!)
- Implement custom filters/muxers
- Debug streaming issues
- Contribute to OpenIPC camera streaming

Good luck! This is a practical data structure you'll use constantly in systems programming.
