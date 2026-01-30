#include "server.h"

// here both server and client run on the same machine
// check if:
// echo works correctly
// no memory leaks
int main(int argc, char **argv) {
  int server_socket = create_udp_socket("8888");
  server_loop(server_socket);

  return 0;
}
