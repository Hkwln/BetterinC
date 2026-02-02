#include <stdio.h>
// here both server and client run on the same machine
// check if:
// echo works correctly
// no memory leaks
// Legacy, this code is merged in server.c and client.c seperatly
int main(int argc, char **argv) {
  printf("--------------starting server---------------------");
  int server_socket = create_udp_socket("8888");
  server_loop(server_socket);
  return 0;
}
