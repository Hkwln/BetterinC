#pragma once
#include <stdint.h>

// TODO: Add PDEP/PEXT optimisation
// ADD test suite
typedef struct {
  uint16_t x;
  uint16_t y;
} coordinates_t;

uint32_t morton_encode(uint16_t x, uint16_t y);
coordinates_t morton_decode(uint32_t both);
