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

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

int create_tcp_socket(char *port) {
  struct socket_addr_in server_in;
  struct addrinfo *res;
  struct addrinfo hints;
  int serverfd = socket(AF_INET, SOCK_DGRAM, 0);

  return 1;
}

int main() {
  int server_socket = create_tcp_socket("8888");
  return 0;
}
