#pragma once

int create_udp_socket(char *port);
void server_loop(int socketfd);
