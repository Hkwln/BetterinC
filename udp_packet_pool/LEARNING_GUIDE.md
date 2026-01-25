# UDP Packet Pool - Learning Guide

## Goal: Build a UDP Echo Server with Memory Pool

Send UDP packets from your **laptop → Ubuntu server → back to laptop**, all using efficient freelist pools!

**YES, this is absolutely doable!** UDP between machines on your local network is simpler than you think.

---

## Phase 1: Core Networking Concepts (2-3 hours reading)

### Essential Topics to Research:

1. **UDP vs TCP**
   - Why UDP? (connectionless, no handshake, perfect for pools!)
   - When to use UDP vs TCP
   - UDP doesn't guarantee delivery (that's okay for echo server)

2. **Socket Programming Basics**
   - What is a socket? (file descriptor for network I/O)
   - IPv4 vs IPv6 (start with IPv4)
   - Port numbers (use 8888 or any > 1024)
   - `INADDR_ANY` vs specific IP

3. **Network Byte Order**
   - Big endian vs little endian
   - `htons()`, `htonl()`, `ntohs()`, `ntohl()` functions
   - Why: Your laptop (x86) is little endian, network is big endian!

### Recommended Reading:
- **Beej's Guide to Network Programming**: https://beej.us/guide/bgnet/
  - Read Chapter 5 (Socket Programming) and 6 (UDP)
  - Focus on `sendto()` and `recvfrom()` examples
- Man pages: `man 7 udp`, `man 2 socket`, `man 2 bind`

---

## Phase 2: Linux Socket API (The library you need!)

### You DON'T need external libraries! Use standard POSIX:

```c
#include <sys/socket.h>  // socket(), bind(), sendto(), recvfrom()
#include <netinet/in.h>  // struct sockaddr_in, INADDR_ANY
#include <arpa/inet.h>   // inet_pton(), inet_ntoa()
#include <unistd.h>      // close()
#include <string.h>      // memset()
```

### Key Functions to Master:

1. **`socket()`** - Create a socket
   ```c
   int socket(int domain, int type, int protocol);
   // AF_INET = IPv4, SOCK_DGRAM = UDP, 0 = default protocol
   ```

2. **`bind()`** - Bind socket to IP:PORT (server side)
   ```c
   int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
   ```

3. **`recvfrom()`** - Receive UDP packet (blocks until data arrives)
   ```c
   ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                    struct sockaddr *src_addr, socklen_t *addrlen);
   ```

4. **`sendto()`** - Send UDP packet (no connection needed!)
   ```c
   ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                  const struct sockaddr *dest_addr, socklen_t addrlen);
   ```

### Important Structs:

```c
struct sockaddr_in {
    sa_family_t    sin_family; // AF_INET
    in_port_t      sin_port;   // Port number (use htons()!)
    struct in_addr sin_addr;   // IP address
    char           sin_zero[8]; // Padding (use memset to zero!)
};
```

**Lookup:** `man 7 ip` for `sockaddr_in` details

---

## Phase 3: Laptop ↔ Server Communication Setup

### Step 1: Find Your Server's IP Address
On your Ubuntu server, run:
```bash
ip addr show
# or
hostname -I
```
Look for something like `192.168.1.100` (local network) or `10.0.0.5`

### Step 2: Test Basic UDP (Before your C program)
On **server**:
```bash
nc -u -l 8888  # Listen UDP on port 8888
```

On **laptop**:
```bash
echo "Hello Server" | nc -u <server-ip> 8888
```

**If you see "Hello Server" on the server terminal**, networking works! 🎉

### Step 3: Firewall Rules (If packets don't arrive)
On **Ubuntu server**:
```bash
sudo ufw allow 8888/udp
# or disable temporarily for testing:
sudo ufw disable
```

### Step 4: Your Goal Architecture
```
[Laptop Client]  --UDP packet--> [Server :8888]
                <--Echo back---
```

**Server never closes connection** (UDP is stateless!)

---

## Phase 4: Memory Pool Integration Strategy

### Packet Buffer Design:

```c
#define PACKET_SIZE 1024

typedef struct {
    char data[PACKET_SIZE];     // Actual packet data
    size_t length;              // How many bytes used
    struct sockaddr_in sender;  // Who sent it (for echo back)
    uint64_t timestamp;         // When received (optional)
} PacketBuffer;
```

### Pool Usage Pattern:

**Server loop:**
1. Allocate buffer from pool: `PacketBuffer* buf = freelist_pool_alloc(pool);`
2. `recvfrom()` into `buf->data`
3. Store sender address in `buf->sender`
4. Process/log packet
5. Echo: `sendto()` back to `buf->sender`
6. Free buffer: `freelist_pool_free(pool, buf);`

**Why pools?** Handle packet bursts without malloc spam!

---

## Phase 5: Advanced Topics (After basic echo works)

### 1. Non-blocking I/O
- Use `fcntl()` to set `O_NONBLOCK` on socket
- `recvfrom()` returns immediately if no data
- Combine with `select()` or `epoll()` for multiple clients

### 2. Multi-threaded Server
- Thread pool pattern (like your memory pool!)
- Each thread handles packets from shared socket
- Mutex around `recvfrom()`? Or use `SO_REUSEPORT`!

### 3. Packet Loss Simulation
```bash
# Add 10% packet loss to test UDP reliability
sudo tc qdisc add dev eth0 root netem loss 10%
```

### 4. Performance Testing
- Benchmark: Send 100k packets, measure latency
- Track pool statistics (allocations/sec)
- Use `time` or `clock_gettime()` for precision

---

## Potential Gotchas (Save yourself hours!)

⚠️ **`bind()` fails with "Address already in use"**
- Another process uses port 8888
- Solution: `sudo netstat -tulpn | grep 8888` to find it
- Or use `SO_REUSEADDR` socket option

⚠️ **Packets don't arrive between machines**
- Firewall blocking (see Step 3)
- Wrong IP address (check with `ping <server-ip>`)
- Server not actually listening (`ss -ulnp | grep 8888`)

⚠️ **`sendto()` succeeds but nothing received**
- UDP doesn't report delivery failure!
- Use `tcpdump` to see actual packets:
  ```bash
  sudo tcpdump -i any port 8888
  ```

⚠️ **Buffer too small**
- UDP max datagram: 65507 bytes (but usually use < 1500 for Ethernet MTU)
- Start with 1024 bytes

⚠️ **Forgetting `htons()`**
- Port numbers MUST be converted!
- `bind()` will succeed but wrong port

---

## Debugging Tools

1. **`netstat`** - See listening sockets
   ```bash
   netstat -tulpn  # t=TCP, u=UDP, l=listening, p=program, n=numeric
   ```

2. **`tcpdump`** - Packet sniffer
   ```bash
   sudo tcpdump -i any -n port 8888
   ```

3. **`strace`** - Trace system calls
   ```bash
   strace -e trace=socket,bind,sendto,recvfrom ./server
   ```

4. **`nc` (netcat)** - Test without your code
   ```bash
   nc -u <ip> <port>  # Client
   nc -u -l <port>    # Server
   ```

---

## Suggested Milestones

### Milestone 1: Local Echo (Same machine)
- Server binds to `127.0.0.1:8888`
- Client sends to `127.0.0.1:8888`
- **Goal:** Echo works on localhost

### Milestone 2: Network Echo (Laptop → Server)
- Server binds to `0.0.0.0:8888` (all interfaces)
- Client sends to `<server-lan-ip>:8888`
- **Goal:** Cross-machine communication

### Milestone 3: Pool Integration
- Replace malloc with freelist
- Pre-allocate 1000 packet buffers
- **Goal:** Measure performance improvement

### Milestone 4: Stress Test
- Send 10k packets rapidly
- Server handles without crash
- **Goal:** Pool prevents malloc overhead

### Milestone 5 (Optional): Daemon Mode
- Server runs in background (`daemon()` or `fork()`)
- Writes PID file to `/var/run/udp_server.pid`
- Logs to `/var/log/udp_server.log`

---

## Example Command Flow

**Terminal 1 (Server on Ubuntu):**
```bash
cd ~/your_project
make server
./server 8888  # Listen on port 8888
# Output: Server listening on 0.0.0.0:8888
```

**Terminal 2 (Client on Laptop):**
```bash
./client <server-ip> 8888 "Hello from laptop!"
# Output: Received echo: "Hello from laptop!"
```

---

## Additional Resources

- **Book:** "Unix Network Programming" by W. Richard Stevens (Vol 1, Chapter 8)
- **Video:** "Socket Programming in C" - CodeVault YouTube
- **Tool:** Wireshark for visual packet inspection
- **RFC 768:** UDP Protocol Specification (short, readable!)

---

## Questions to Answer in Your README.md

After completing the project:
1. How much faster is pool allocation vs malloc for 100k packets?
2. What happens if you send faster than the server can echo?
3. How does UDP differ from TCP in your testing?
4. What's the maximum packet size before fragmentation?
5. Can you handle multiple clients simultaneously? How?

---

## Pro Tips

✅ Start with the simplest version (local echo, fixed buffer)
✅ Add one feature at a time (pool, then network, then multi-client)
✅ Use `printf()` debugging liberally at first
✅ Test with `nc` before debugging your C code
✅ Read the man pages! They have examples: `man 7 udp`
✅ Your freelist pool is already perfect for this - just use it!

**You got this!** 🚀 UDP is simpler than TCP, and you already have the hardest part (the pool) working.
