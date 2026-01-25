#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

// TODO: implement these functions:
void create_udp_socket(int port) {
  struct sockaddr_storage their_addr;
  int socketfd, new_fd;
  socklen_t addr_size;
  struct addrinfo *res;
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  int status;
  if ((status = getaddrinfo(NULL, &port, &hints, &res)) != 0) {
    fprintf(stderr, "gai error: %s\n", gai_strerror(status));
    exit(1);
  }
  socketfd = socket(AF_INET, SOCK_DGRAM, 0);
  bind(socketfd, "192.168.0.5:8888", res->ai_addrlen);
  listen(socketfd, 5);
  addr_size = sizeof their_addr;
  new_fd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_size);
}

void server_loop(int newsocket, struct sockaddr_storage) {
  void *buf = malloc(1024);

  while (1) {
    recvfrom(newsocket, *buf, 1024, 0, );
    print("received data: %s", data);
  }
}
