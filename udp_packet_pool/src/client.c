#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
int main(int argc, char **argv) {
  // 4 = programm + 3 argumente
  if (argc != 4) {
    fprintf(stderr, "Usage: ./client <ip> <port> <message> \n");
    return 0;
  }
  struct sockaddr_in send;
  char *ip = argv[1];
  int s = inet_pton(AF_INET, ip, &send.sin_addr);
  if (s <= 0) {
    if (s == 0)
      fprintf(stderr, "Not in presentation format");
    else
      perror("inet_pton");
    exit(EXIT_FAILURE);
  }
  uint16_t port_num = atoi(argv[2]);
  send.sin_port = htons(port_num);
  char *message = argv[3];
  send.sin_family = AF_INET;
  int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socketfd == -1) {
    fprintf(stderr, "%d: %s \n", errno, strerror(errno));
    return 0;
  }
  socklen_t sender_len = sizeof(send);
  if (sendto(socketfd, message, strlen(message), 0, (struct sockaddr *)&send,
             sender_len) == -1) {
    fprintf(stderr, "%d: %s \n", errno, strerror(errno));
    return 0;
  }
  // receive the echo message from the server
  // TODO: implement a timer if server is unreachable
  char buf[strlen(message) + 1];
  ssize_t bytes_received = recvfrom(socketfd, buf, sizeof(buf), 0,
                                    (struct sockaddr *)&send, &sender_len);
  if (bytes_received == -1) {
    fprintf(stderr, "%d: %s \n", errno, strerror(errno));
    return 0;
  }
  buf[bytes_received] = '\0';
  printf("Received your echoed message: \n%s \n", buf);
  return 0;
}
