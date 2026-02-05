#pragma once
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#define PACKET_SIZE 1024
typedef struct {
  uint8_t data[PACKET_SIZE];
  size_t length;
  struct sockaddr_in *sender;
  // Add more fields as needed
} PacketBuffer;
