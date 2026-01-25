# Implementation Checklist

## Phase 1: Setup & Learning ⏱️ 3-4 hours

- Read `LEARNING_GUIDE.md` completely
- Read Beej's Guide Chapter 5 & 6 (UDP sections)
- Read `man 7 udp` and `man 2 socket`
- [ ] Test basic UDP with netcat:

  ```bash
  # Terminal 1: nc -u -l 8888
  # Terminal 2: echo "test" | nc -u 127.0.0.1 8888
  ```

- [ ] Find Ubuntu server IP: `hostname -I`
- [ ] Test laptop → server with netcat
- [ ] Configure firewall: `sudo ufw allow 8888/udp`

---

## Phase 2: Basic UDP Server (Localhost) ⏱️ 3-4 hours

### Server Implementation

- [ ] Create `server.c` with minimal includes
- [ ] Implement `create_udp_socket(int port)` function
  - [ ] `socket(AF_INET, SOCK_DGRAM, 0)`
  - [ ] Setup `sockaddr_in` struct
  - [ ] `bind()` to `127.0.0.1:8888`
  - [ ] Error handling for all calls
- [ ] Implement `server_loop()` function
  - [ ] Allocate fixed 1024-byte buffer (malloc for now)
  - [ ] Call `recvfrom()` in infinite loop
  - [ ] Print received data
  - [ ] `sendto()` echo back to sender
- [ ] Compile and test on localhost
- [ ] Handle Ctrl+C gracefully (`signal(SIGINT, handler)`)

### Client Implementation

- [ ] Create `client.c` with argument parsing
  - [ ] Usage: `./client <ip> <port> <message>`
- [ ] Create UDP socket (no bind needed!)
- [ ] Setup destination `sockaddr_in`
- [ ] `sendto()` message to server
- [ ] `recvfrom()` wait for echo (with timeout!)
- [ ] Print echoed message
- [ ] Compare sent vs received data

### Testing

- [ ] Server and client run on same machine
- [ ] Echo works correctly
- [ ] No memory leaks (`valgrind --leak-check=full ./server`)

---

## Phase 3: Network Communication ⏱️ 2-3 hours

- [ ] Change server to bind `0.0.0.0` (all interfaces)
- [ ] Deploy server to Ubuntu machine
- [ ] Run client on laptop with server's LAN IP
- [ ] Verify cross-machine echo works
- [ ] Test with various message sizes (1 byte, 512 bytes, 1024 bytes)
- [ ] Use `tcpdump` to observe packets:

  ```bash
  sudo tcpdump -i any port 8888 -X
  ```

### Debugging (if packets don't arrive)

- [ ] `ping` server from laptop
- [ ] Check firewall on both machines
- [ ] Verify server is listening: `ss -ulnp | grep 8888`
- [ ] Check routing/NAT issues

---

## Phase 4: Freelist Pool Integration ⏱️ 3-4 hours

### Copy Pool Code

- [ ] Copy `../memory_pool/freelist.h` and `freelist.c` to this folder
- [ ] Or link them in Makefile

### Define PacketBuffer Struct

```c
#define PACKET_SIZE 1024

typedef struct {
    uint8_t data[PACKET_SIZE];
    size_t length;
    struct sockaddr_in sender;
    // Add more fields as needed
} PacketBuffer;
```

### Integrate Pool

- [ ] Create pool at server startup: `freelist_pool_create(sizeof(PacketBuffer), 1000)`
- [ ] Replace malloc with `freelist_pool_alloc(pool)`
- [ ] Replace free with `freelist_pool_free(pool, buf)`
- [ ] Test that echo still works
- [ ] Print pool stats every 1000 packets

---

## Phase 5: Performance Benchmarking ⏱️ 2-3 hours

### Benchmark 1: Allocation Speed

- [ ] Create `benchmark_alloc.c`
- [ ] Time 1 million malloc+free cycles
- [ ] Time 1 million pool alloc+free cycles
- [ ] Calculate speedup factor
- [ ] Record results in README.md

### Benchmark 2: Throughput

- [ ] Modify client to send 10k packets rapidly
- [ ] Server counts packets/sec
- [ ] Measure with pool
- [ ] Measure with malloc (modify server)
- [ ] Compare results

### Benchmark 3: Latency

- [ ] Add timestamp to packets
- [ ] Client measures round-trip time
- [ ] Calculate min/avg/max/p99 latency
- [ ] Test under different loads

### Benchmark 4: Pool Exhaustion

- [ ] Reduce pool size to 100 buffers
- [ ] Send 1000 packets simultaneously
- [ ] Observe pool exhaustion behavior
- [ ] Add pool stats: `allocated_count`, `capacity`

---

## Phase 6: Advanced Features (Optional) ⏱️ 5-10 hours

### Multi-threading

- [ ] Create thread pool (4 threads)
- [ ] Each thread calls `recvfrom()`
- [ ] Add mutex around pool operations
- [ ] Or use per-thread pools (better!)
- [ ] Benchmark multi-threaded performance

### Protocol Design

- [ ] Define packet header:

  ```c
  struct PacketHeader {
      uint32_t sequence_number;
      uint16_t payload_length;
      uint16_t checksum;
  };
  ```

- [ ] Validate packets on server
- [ ] Add different message types (echo, ping, stats)

### Daemon Mode

- [ ] Implement `daemonize()` function
  - [ ] `fork()` and parent exits
  - [ ] `setsid()` to create new session
  - [ ] `chdir("/")` to avoid blocking unmount
  - [ ] Close stdin/stdout/stderr
  - [ ] Redirect to `/dev/null`
- [ ] Write PID file to `/tmp/udp_server.pid`
- [ ] Log to syslog instead of stdout
- [ ] Control via signals (SIGHUP = reload config)

### Packet Loss Resilience

- [ ] Client retries if no echo after timeout
- [ ] Server detects duplicate packets (sequence numbers)
- [ ] Implement basic flow control

### IPv6 Support

- [ ] Use `AF_INET6` for IPv6
- [ ] Test with `::1` (IPv6 localhost)
- [ ] Dual-stack server (both IPv4 and IPv6)

---

## Phase 7: Polish & Documentation ⏱️ 2-3 hours

- [ ] Add comprehensive error messages
- [ ] Implement proper logging (log level, timestamps)
- [ ] Create `Makefile` with targets:
  - [ ] `make all` - build server and client
  - [ ] `make server` - build server only
  - [ ] `make client` - build client only
  - [ ] `make clean` - remove binaries
  - [ ] `make test` - run automated tests
- [ ] Write comprehensive README.md:
  - [ ] Project description
  - [ ] Compilation instructions
  - [ ] Usage examples
  - [ ] Benchmark results
  - [ ] Architecture diagram (ASCII art)
  - [ ] Lessons learned
- [ ] Add command-line help (`--help` flag)
- [ ] Create systemd service file (optional):

  ```ini
  [Unit]
  Description=UDP Echo Server
  After=network.target

  [Service]
  Type=simple
  ExecStart=/path/to/server 8888
  Restart=always

  [Install]
  WantedBy=multi-user.target
  ```

---

## Testing Checklist

- [ ] Works on localhost (127.0.0.1)
- [ ] Works across machines (LAN)
- [ ] Handles small packets (1 byte)
- [ ] Handles large packets (1024 bytes)
- [ ] Handles rapid packet bursts
- [ ] No memory leaks (valgrind clean)
- [ ] No buffer overflows (test with long inputs)
- [ ] Graceful shutdown (Ctrl+C)
- [ ] Pool exhaustion handled (doesn't crash)
- [ ] Invalid inputs rejected (wrong IP format, etc.)

---

## Questions to Answer (in README.md)

1. **Performance:** How much faster is the pool vs malloc?
2. **Pool size:** What's the minimum pool size needed for your workload?
3. **Packet loss:** What happens when UDP packets are lost?
4. **Scaling:** How many packets/sec can your server handle?
5. **Bottleneck:** What limits throughput? (CPU, network, pool size?)
6. **Multi-threading:** Does it improve performance? By how much?
7. **Latency:** What's the typical round-trip time on your LAN?

---

## Stretch Goals

- [ ] Build a simple chat application on top of this
- [ ] Add encryption (integrate libsodium)
- [ ] Implement reliable UDP (acknowledgments, retransmission)
- [ ] Create a web dashboard (server exposes stats via HTTP)
- [ ] Port to Windows (Winsock API)
- [ ] Add support for broadcast/multicast
- [ ] Integrate with your circular buffer for packet queue
- [ ] Integrate with your hash table for client tracking

---

## Notes & Reflections

(Use this space to track challenges, solutions, and insights as you build)

-

---

## Time Tracking

- **Phase 1 (Setup):** \_\_\_ hours
- **Phase 2 (Basic Server):** \_\_\_ hours
- **Phase 3 (Network):** \_\_\_ hours
- **Phase 4 (Pool Integration):** \_\_\_ hours
- **Phase 5 (Benchmarks):** \_\_\_ hours
- **Phase 6 (Advanced):** \_\_\_ hours
- **Phase 7 (Polish):** \_\_\_ hours
- **Total:** \_\_\_ hours

**Start Date:** ****\_\_\_****
**End Date:** ****\_\_\_****
