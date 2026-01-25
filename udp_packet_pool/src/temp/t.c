// this is just a testground for testing, just to test some functions
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
int main(int argc, char **argv) {

  // how do i uncomment multiple lines with nvim
  // const char *ip = "192.168.0.5";
  const char *service = "9431"; // port number oder protocol(http)
                                // XXX: was macht folgendes?
  char ipstr[INET6_ADDRSTRLEN];
  struct addrinfo *res, *p;
  int status;
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;     // dont care if ip4 or ip6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
  if (argc != 2) {
    fprintf(stderr, "usage: showip hostname\n");
    return 0;
  }
  if ((status = getaddrinfo(NULL, service, &hints, &res)) != 0) {
    fprintf(stderr, "gai error: %s\n", gai_strerror(status));
    exit(1);
  }
  printf("IP address for %s:\n", argv[1]);
  for (p = res; p != NULL; p = p->ai_next) {
    char *ipver;
    if (p->ai_family == AF_INET) {
      ipver = "IPV4";

    } else {
      ipver = "IPV4";
    }

    printf(" %s: %s \n", ipver, ipstr);
  }
  freeaddrinfo(res); // free linked list
  return 0;
}
