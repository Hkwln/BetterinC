#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
/* ERSTELLT SOCKET, BINDET IHN, GIBT DEN SOCKET FILE DESCRIPOR ZURÜCK*/
int create_udp_socket(char *port) {

  int socketfd;
  struct addrinfo *res;
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  // res->ai_family, res->ai_socktype, res->ai_protocol
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  int status;
  if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
    fprintf(stderr, "gai error: %s\n", gai_strerror(status));
    exit(1);
  }
  socketfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socketfd == -1) {

    fprintf(stderr, "%d: %s \n", errno, strerror(errno));
    exit(1);
  }
  if (bind(socketfd, res->ai_addr, res->ai_addrlen) == -1) {
    fprintf(stderr, "%d: %s \n", errno, strerror(errno));
    exit(1);
  }
  freeaddrinfo(res);
  return socketfd;
}
/* NIMMT DEN SOCKET FD UND LÄUFT IN EINER ENDLOSSCHLEIFE*/
void server_loop(int socketfd) {
  // change to memory pool in the future
  void *buf = malloc(1024);
  while (1) {
    // ich brauhce *socketlen für den letzten parameter
    struct sockaddr_storage sender;
    socklen_t sender_len = sizeof(sender);
    ssize_t bytes_received = recvfrom(socketfd, buf, 1024, 0,
                                      (struct sockaddr *)&sender, &sender_len);
    if (bytes_received == -1) {
      fprintf(stderr, " %d : %s \n", errno, strerror(errno));
      exit(1);
    }
    buf[bytes_received] = '\0';
    printf("received  %zu bytes: %s \n", bytes_received, (char *)buf);
    sendto(socketfd, buf, bytes_received, 0, (struct sockaddr *)&sender,
           sender_len);
  }
  free(buf);
}
