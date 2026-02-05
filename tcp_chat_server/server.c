/*
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
 * */
#include <sys/socket.h>

int main() {
  int serverfd = socket(AF_INET, SOCK_DGRAM, 0);
  return 0;
}
