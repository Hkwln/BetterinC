#pragma once
#include "data.h"
#include <stdbool.h>
#include <stdint.h>

void bitmap_fill_rect(Bitmap *bmp, uint32_t x, uint32_t y, uint32_t width,
                      uint32_t height, bool white);
void bitmap_draw_hline(Bitmap *bmp, uint32_t x, uint32_t y, uint32_t length,
                       bool white);

// Box drawing animation state
typedef struct {
  int x, y;          // Position
  int width, height; // Ziel-Größe
  int progress;      // 0-100: Animation fortschritt
  bool complete;     // Ist Animation fertig?
  int *draw_order;   // Random order für Pixel
  int total_pixels;  // Anzahl Pixel im Rahmen
  int drawn_pixels;  // Wie viele bereits gezeichnet
} BoxAnimation;

// Box Animation initialisieren
BoxAnimation box_anim_init(int x, int y, int width, int height);

// Ein Frame der Box-Animation auf die Bitmap zeichnen
// Gibt true zurück wenn Animation komplett ist
bool box_anim_draw_frame(BoxAnimation *anim, Bitmap *bmp, int speed, int e);

// Box-Animation aufräumen
void box_anim_free(BoxAnimation *anim);
