#include "data.h"
#include "freelistcpy/freelist.h"
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
// TODO: 27 statt bind zu NULL bind zu 127.0.0.1;
// no signal handler for ctrl +c
//
/*Funtkioniert folgender test wurde mit python im terminal gemacht während
 server im anderen terminal lief:
 * python3 -c "
     import socket
     s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
     s.sendto(b'Test123', ('127.0.0.1', 8888))
     print(s.recvfrom(1024))
     "*/
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
void server_loop(int socketfd, FreeListPool *Packet) {
  // change to memory pool in the future
  PacketBuffer *buf = freelist_pool_alloc(Packet);
  while (1) {
    // ich brauhce *socketlen für den letzten parameter
    struct sockaddr_storage sender;
    socklen_t sender_len = sizeof(sender);
    buf->length = recvfrom(socketfd, buf->data, 1024, 0,
                           (struct sockaddr *)&sender, &sender_len);

    if (buf->length == -1) {
      fprintf(stderr, " %d : %s \n", errno, strerror(errno));
      exit(1);
    }
    // sender = buf->sender;
    ((char *)buf)[buf->length] = '\0';
    printf("received  %zu bytes: %s \n", buf->length, (char *)buf);
    sendto(socketfd, buf, buf->length, 0, (struct sockaddr *)&sender,
           sender_len);
  }
  freelist_pool_free(Packet, buf);
}
int main(int argc, char **argv) {
  int server_socket = create_udp_socket("8888");
  FreeListPool *Packet = freelist_pool_create(sizeof(PacketBuffer), 1000);
  server_loop(server_socket, Packet);
  freelist_pool_destroy(Packet);
  return 0;
}
