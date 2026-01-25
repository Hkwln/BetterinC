#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
int main(void) {
  int s = 0;
  struct addrinfo *hints, *res;
  getaddrinfo("www.example.com", "http", &hints, &res);
  s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  bind(s, res->ai_addr, res->ai_addrlen);
  // INFO: socket,connect, listen, accept and bind return -1 if they fail, and
  // sets errno to its error value
  return 0;
}
