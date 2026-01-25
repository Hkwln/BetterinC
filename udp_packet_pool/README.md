# UDP Packet Pool

High-performance UDP echo server using freelist memory pool for zero-allocation packet handling.

## Project Goals

1. **Learn UDP socket programming** in C
2. **Apply freelist memory pool** to real-world networking
3. **Benchmark performance** - pool vs malloc
4. **Network communication** between laptop and Ubuntu server

## Structure

- `LEARNING_GUIDE.md` - Comprehensive learning path with resources
- `ARCHITECTURE.md` - System design and data flow diagrams
- `TODO.md` - Implementation checklist and milestones

## Quick Start

**Read in this order:**
1. `LEARNING_GUIDE.md` - Start here! Essential concepts and resources
2. `ARCHITECTURE.md` - Understand the system design
3. `TODO.md` - Track your implementation progress

## Resources

- Beej's Guide to Network Programming: https://beej.us/guide/bgnet/
- Man pages: `man 7 udp`, `man 2 socket`, `man 2 bind`
- Your completed freelist pool: `../memory_pool/freelist.c`

## Expected Performance

- **Throughput:** 50k-100k packets/sec (single-threaded)
- **Latency:** < 1ms round-trip on LAN
- **Pool speedup:** 10-100x faster than malloc

## Hardware Requirements

- Laptop (client)
- Ubuntu server (echo server) on same LAN
- Network connectivity between them

## Author Notes

Build this yourself! Don't skip to copying examples. Understanding beats completion speed.

Good luck! 🚀
