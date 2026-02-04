# Coding Exercise: TCP Chat Server in C

**Difficulty:** Advanced-Intermediate  
**Topic:** TCP Sockets + `poll()`/`select()` + Concurrent Connections + Protocol Design  
**Estimated Time:** 3-5 Tage (15-25 Stunden)

---

## Hintergrund

Du hast UDP mit deinem `udp_packet_pool` Projekt kennengelernt. Jetzt ist es Zeit für **TCP** – das verbindungsorientierte Protokoll. Im Gegensatz zu UDP:

- **Verbindungsbasiert:** Client und Server müssen erst eine Verbindung aufbauen
- **Zuverlässig:** Pakete kommen garantiert an, in der richtigen Reihenfolge
- **Stream-basiert:** Keine Nachrichtengrenzen (du musst selbst definieren wo eine Nachricht endet!)

**Warum ein Chat-Server?**
- Mehrere gleichzeitige Verbindungen → `poll()` oder `select()` lernen
- Nachrichten zwischen Clients weiterleiten → Protokoll-Design
- Praktisch: Du kannst ihn wirklich nutzen!

---

## Projektziel

Baue einen **einfachen Chat-Server**, der:
1. Mehrere Clients gleichzeitig akzeptiert
2. Nachrichten von einem Client an alle anderen weiterleitet
3. Mit `poll()` oder `select()` arbeitet (kein Multi-Threading!)
4. Ein eigenes simples Protokoll verwendet

---

## Teil 1: TCP Grundlagen verstehen (2-3 Stunden)

### Die wichtigsten Unterschiede zu UDP

```
UDP: socket() → bind() → recvfrom()/sendto()

TCP Server: socket() → bind() → listen() → accept() → recv()/send()
TCP Client: socket() → connect() → recv()/send()
```

### Lesen & Verstehen

**Pflichtlektüre:**
- Beej's Guide, Kapitel 5.1-5.4 (TCP Sockets): https://beej.us/guide/bgnet/
- `man 2 listen`, `man 2 accept`, `man 2 poll`

**Fragen zum Selbsttest (beantworte ohne nachzuschauen):**
1. Was macht `listen()` genau? Was bedeutet der "backlog" Parameter?
2. Warum blockiert `accept()`? Was gibt es zurück?
3. Was passiert wenn du `recv()` auf einem TCP Socket aufrufst und noch keine Daten da sind?
4. Warum braucht TCP keine `sendto()`/`recvfrom()`?

---

## Teil 2: Minimaler TCP Echo Server (3-4 Stunden)

### Aufgabe 2.1: Single-Client Echo Server

Baue zuerst einen Server, der nur **einen** Client gleichzeitig bedient:

```
Pseudocode:
    server_fd = socket(TCP)
    bind(server_fd, port 9999)
    listen(server_fd, backlog=5)
    
    client_fd = accept(server_fd)  // Blockiert bis Client kommt
    
    while True:
        data = recv(client_fd)
        if data == 0: break  // Client disconnected
        send(client_fd, data)  // Echo zurück
    
    close(client_fd)
    close(server_fd)
```

### Aufgabe 2.2: Testen mit netcat

```bash
# Terminal 1: Server starten
./server

# Terminal 2: Client mit netcat
nc localhost 9999
Hello!  # tippen → sollte "Hello!" zurückkommen
```

### Aufgabe 2.3: Wireshark/tcpdump (Optional aber empfohlen!)

```bash
sudo tcpdump -i lo port 9999 -X
```

Beobachte:
- **3-Way Handshake:** SYN → SYN-ACK → ACK
- **Daten:** PSH, ACK Pakete
- **Verbindungsende:** FIN → ACK

---

## Teil 3: Multiplexing mit `poll()` (4-6 Stunden)

### Das Problem

Mit nur einem `accept()` und einem `recv()` kannst du nur einen Client bedienen. Aber ein Chat-Server braucht viele gleichzeitige Verbindungen!

### Die Lösung: `poll()` (oder `select()`)

`poll()` überwacht mehrere File Descriptors gleichzeitig und sagt dir, welche "ready" sind.

```c
struct pollfd fds[MAX_CLIENTS + 1];  // +1 für server socket

// Server socket überwachen (für neue Verbindungen)
fds[0].fd = server_fd;
fds[0].events = POLLIN;

// Client sockets überwachen (für Daten)
for (int i = 0; i < num_clients; i++) {
    fds[i+1].fd = clients[i];
    fds[i+1].events = POLLIN;
}

int ready = poll(fds, num_fds, timeout_ms);

// Jetzt prüfen welche FDs ready sind
if (fds[0].revents & POLLIN) {
    // Neuer Client wartet! → accept()
}
for (int i = 0; i < num_clients; i++) {
    if (fds[i+1].revents & POLLIN) {
        // Client i hat Daten geschickt → recv()
    }
}
```

### Aufgabe 3.1: Multi-Client Echo Server

Erweitere deinen Server:

```
Pseudocode:
    clients[] = leeres Array
    
    while True:
        poll(server_fd, alle clients)
        
        if server_fd ready:
            new_client = accept()
            clients.append(new_client)
            print("Neuer Client verbunden")
        
        for each client in clients:
            if client ready:
                data = recv(client)
                if data == 0:
                    clients.remove(client)
                    close(client)
                    print("Client disconnected")
                else:
                    send(client, data)  // Echo
```

### Aufgabe 3.2: Testen

```bash
# Terminal 1: Server
./server

# Terminal 2: Client A
nc localhost 9999

# Terminal 3: Client B  
nc localhost 9999

# Beide können jetzt gleichzeitig kommunizieren!
```

---

## Teil 4: Chat-Protokoll Design (2-3 Stunden)

### Das Problem: TCP ist ein Stream!

Bei TCP gibt es keine "Nachrichten" – nur ein endloser Bytestrom. Wenn Client A "Hello" und "World" schickt, könnte der Server empfangen:
- `"HelloWorld"` (zusammen)
- `"Hel"` + `"loWorld"` (getrennt)
- `"H"` + `"ello"` + `"Wor"` + `"ld"` (beliebig)

**Du musst selbst definieren, wo eine Nachricht endet!**

### Lösung: Einfaches Protokoll

**Option A: Newline-Terminated**
```
Hello\n
World\n
```
→ Einfach, aber was wenn Nachricht selbst `\n` enthält?

**Option B: Length-Prefix**
```
[2 bytes length][message data]
\x00\x05Hello
\x00\x05World
```
→ Robuster, aber komplexer

### Aufgabe 4.1: Protokoll wählen und implementieren

Für dieses Projekt: **Newline-Terminated** (einfacher)

```c
// Buffer pro Client
char buffer[MAX_CLIENTS][1024];
int buffer_len[MAX_CLIENTS];

// recv() fügt Daten zum Buffer hinzu
int bytes = recv(client_fd, buf + len, sizeof(buf) - len, 0);
buffer_len[i] += bytes;

// Nach '\n' suchen
char* newline = memchr(buffer[i], '\n', buffer_len[i]);
if (newline) {
    // Vollständige Nachricht gefunden!
    size_t msg_len = newline - buffer[i] + 1;
    process_message(buffer[i], msg_len);
    
    // Rest des Buffers nach vorne schieben
    memmove(buffer[i], newline + 1, buffer_len[i] - msg_len);
    buffer_len[i] -= msg_len;
}
```

---

## Teil 5: Broadcast (Nachrichten an alle) (2-3 Stunden)

### Aufgabe 5.1: Implementiere Broadcast

Wenn Client A eine Nachricht schickt, sende sie an alle anderen Clients:

```
Pseudocode:
    def broadcast(sender, message):
        for client in clients:
            if client != sender:
                send(client, message)
```

### Aufgabe 5.2: Usernames

Erweitere das Protokoll:
1. Erster Befehl muss `/nick <name>` sein
2. Nachrichten werden als `[username]: message` gebroadcastet

```
Client → Server: /nick Paul
Server → Client: Welcome, Paul!
Client → Server: Hello everyone!
Server → alle anderen: [Paul]: Hello everyone!
```

---

## Teil 6: Integration mit deinen Projekten (Optional, 3-5 Stunden)

### Circular Buffer für Client-Nachrichten

Nutze deinen `circular_buffer` für den Empfangs-Buffer jedes Clients:

```c
typedef struct {
    int fd;
    char username[32];
    CircularBuffer* recv_buffer;
} Client;
```

### Freelist Pool für Client-Structs

```c
FreeListPool* client_pool;

// Bei accept():
Client* client = freelist_pool_alloc(client_pool);

// Bei disconnect:
freelist_pool_free(client_pool, client);
```

---

## Ressourcen

### Bücher
- **Beej's Guide to Network Programming** (Pflicht!): https://beej.us/guide/bgnet/
- **Unix Network Programming** by W. Richard Stevens (Klassiker, sehr detailliert)

### Man Pages
```bash
man 2 socket
man 2 bind
man 2 listen
man 2 accept
man 2 poll
man 2 select
man 7 tcp
```

### Online
- https://www.ibm.com/docs/en/i/7.3?topic=designs-example-nonblocking-io-select (IBM poll() Tutorial)

---

## Checkliste

### Phase 1: Basics
- [ ] Single-Client Echo Server funktioniert
- [ ] `tcpdump` zeigt 3-Way Handshake
- [ ] Kann Daten senden/empfangen

### Phase 2: Multi-Client
- [ ] `poll()` loop implementiert
- [ ] Mehrere Clients gleichzeitig verbunden
- [ ] Client disconnect wird erkannt

### Phase 3: Chat
- [ ] Newline-basiertes Protokoll
- [ ] Partial recv() wird korrekt gehandled
- [ ] Broadcast an alle Clients

### Phase 4: Features
- [ ] `/nick` Befehl
- [ ] Nachrichten zeigen Absender
- [ ] Graceful shutdown (Ctrl+C)

### Phase 5: Quality
- [ ] Kein Memory Leak (valgrind)
- [ ] Fehlerbehandlung für alle syscalls
- [ ] README.md mit Usage

---

## Erwartete Struktur

```
tcp_chat_server/
├── Exercise.md         (diese Datei)
├── src/
│   ├── server.c
│   ├── client.c        (optional, kannst auch netcat nutzen)
│   └── protocol.h
├── Makefile
└── README.md
```

---

## Was du lernst

✓ **TCP Socket API** – Der Standard für Netzwerkprogrammierung  
✓ **poll()/select()** – I/O Multiplexing ohne Threads  
✓ **Stream-basierte Protokolle** – Message Framing  
✓ **State Management** – Buffer pro Client  
✓ **Reale Anwendung** – Ein funktionierender Chat!

---

## Hinweise

- **Start simple:** Erst Echo, dann Multi-Client, dann Chat
- **Print alles:** `printf("accept: new client fd=%d\n", fd);`
- **Testen mit netcat:** `nc` ist dein bester Freund
- **Fehler prüfen:** JEDER syscall kann fehlschlagen!
- **Non-blocking?** Für dieses Projekt nicht nötig, `poll()` reicht

---

## Vergleich zu UDP

| Aspekt | UDP (dein Projekt) | TCP (dieses Projekt) |
|--------|-------------------|---------------------|
| Verbindung | Keine | 3-Way Handshake |
| Zuverlässigkeit | Pakete können verloren gehen | Garantiert |
| Nachrichten | Klar getrennt | Stream (du musst trennen) |
| Performance | Schneller | Overhead für Zuverlässigkeit |
| Use Case | Gaming, Streaming | Chat, HTTP, Dateitransfer |

Nach diesem Projekt verstehst du beide Seiten der Socket-Programmierung! 🚀
