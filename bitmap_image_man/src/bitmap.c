#include "bitmap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/* TODO:
 * - ADD BOUNDS CHECKING
 * - ADD NULL CHECK AFTER MALLOC/CALLOC*/
Bitmap *bitmap_create(uint32_t width, uint32_t height) {
  // allocate memory to bitmap
  Bitmap *bitmap = malloc(sizeof(Bitmap));
  bitmap->data = calloc(bytes_needed(width, height), 1);
  bitmap->width = width;
  bitmap->height = height;
  return bitmap;
}
void bitmap_destroy(Bitmap *bmp) {
  free(bmp->data);
  free(bmp);
  bmp->height = 0;
  bmp->width = 0;
  bmp->data = NULL;
}

void bitmap_set_pixel(Bitmap *bmp, uint32_t x, uint32_t y, bool white) {
  uint32_t bit_index = y * bmp->width + x;
  uint16_t byte_index = bit_index / 8;
  int16_t bit_offset = bit_index % 8;
  if (white) {
    bmp->data[byte_index] |= (1 << bit_offset);
  } else {
    bmp->data[byte_index] &= ~(1 << bit_offset);
  }
}

bool bitmap_get_pixel(Bitmap *bmp, uint32_t x, uint32_t y) {
  uint32_t bit_index = y * bmp->width + x;
  uint16_t byte_index = bit_index / 8;
  int16_t bit_offset = bit_index % 8;
  return (bmp->data[byte_index] >> bit_offset) & 1;
}
void print_bitmap(Bitmap *bmp) {
#if 1
  // mor efficient buffer printing
  char *buf = malloc(bmp->width * bmp->height + bmp->width + 1);
  int idx = 0;
  for (int h = 0; h < bmp->height; h++) {
    for (int w = 0; w < bmp->width; w++) {
      buf[idx++] = bitmap_get_pixel(bmp, w, h) ? '*' : ' ';  // FIX: w, h statt h, w!
    }
    buf[idx++] = '\n';
  }
  buf[idx++] = '\0';
  printf("%s \n", buf);
  free(buf);

#endif
#if 0
  for (int h = 0; h < bmp->height; h++) {
    for (int w = 0; w < bmp->width; w++) {
      bool pixel = bitmap_get_pixel(bmp, w, h);
      printf("%c", pixel ? '#' : '.');
    }
    printf("\n");
  }
#endif
}

// Print bitmap MIT Box-Overlay im Buffer (mit Unicode Box-Zeichen)
void print_bitmap_with_box(Bitmap *bmp, void *box_ptr) {
  if (!box_ptr) {
    print_bitmap(bmp);
    return;
  }
  
  // Forward declaration aus draw.h
  typedef struct {
    int x, y;
    int width, height;
    int progress;
    bool complete;
    int *draw_order;
    int total_pixels;
    int drawn_pixels;
  } BoxAnimation;
  
  BoxAnimation *box = (BoxAnimation*)box_ptr;
  
  // Größerer Buffer für Unicode (3 bytes pro Unicode-Zeichen)
  char *buf = malloc((bmp->width * 3 + 1) * bmp->height + 10);
  int idx = 0;
  
  for (int h = 0; h < bmp->height; h++) {
    for (int w = 0; w < bmp->width; w++) {
      bool is_game = bitmap_get_pixel(bmp, w, h);
      bool is_box = false;
      int box_type = 0; // 0=none, 1=horizontal, 2=vertical, 3=corner
      
      // Check ob Box-Pixel (nur wenn nicht complete)
      if (!box->complete) {
        int rel_x = w - box->x;
        int rel_y = h - box->y;
        
        // Ist Position im Box-Bereich?
        if (w >= box->x && w < box->x + box->width && 
            h >= box->y && h < box->y + box->height) {
          
          // Check welcher Teil der Box
          bool is_top = (h == box->y);
          bool is_bottom = (h == box->y + box->height - 1);
          bool is_left = (w == box->x);
          bool is_right = (w == box->x + box->width - 1);
          
          if (is_top || is_bottom || is_left || is_right) {
            // Check ob dieses Pixel bereits gezeichnet wurde
            int encoded = rel_x + rel_y * 1000;
            for (int i = 0; i < box->drawn_pixels; i++) {
              if (box->draw_order[i] == encoded) {
                is_box = true;
                
                // Bestimme Box-Typ für Unicode-Zeichen
                if ((is_top || is_bottom) && !is_left && !is_right) {
                  box_type = 1; // horizontal ─
                } else if ((is_left || is_right) && !is_top && !is_bottom) {
                  box_type = 2; // vertical │
                } else {
                  box_type = 3; // corner (vereinfacht)
                }
                break;
              }
            }
          }
        }
      }
      
      // Zeichne Zeichen
      if (is_box) {
        // Unicode Box-Zeichen (UTF-8 kodiert)
        if (box_type == 1) {
          // ─ (U+2500)
          buf[idx++] = 0xE2;
          buf[idx++] = 0x94;
          buf[idx++] = 0x80;
        } else if (box_type == 2) {
          // │ (U+2502)
          buf[idx++] = 0xE2;
          buf[idx++] = 0x94;
          buf[idx++] = 0x82;
        } else if (box_type == 3) {
          // Ecken - bestimme welche
          int rel_x = w - box->x;
          int rel_y = h - box->y;
          
          if (rel_x == 0 && rel_y == 0) {
            // ┌ Oben links (U+250C)
            buf[idx++] = 0xE2;
            buf[idx++] = 0x94;
            buf[idx++] = 0x8C;
          } else if (rel_x == box->width - 1 && rel_y == 0) {
            // ┐ Oben rechts (U+2510)
            buf[idx++] = 0xE2;
            buf[idx++] = 0x94;
            buf[idx++] = 0x90;
          } else if (rel_x == 0 && rel_y == box->height - 1) {
            // └ Unten links (U+2514)
            buf[idx++] = 0xE2;
            buf[idx++] = 0x94;
            buf[idx++] = 0x94;
          } else if (rel_x == box->width - 1 && rel_y == box->height - 1) {
            // ┘ Unten rechts (U+2518)
            buf[idx++] = 0xE2;
            buf[idx++] = 0x94;
            buf[idx++] = 0x98;
          } else {
            // Fallback
            buf[idx++] = '+';
          }
        } else {
          buf[idx++] = '+';
        }
      } else if (is_game) {
        buf[idx++] = '*';
      } else {
        buf[idx++] = ' ';
      }
    }
    buf[idx++] = '\n';
  }
  buf[idx++] = '\0';
  
  printf("%s \n", buf);
  free(buf);
}

// test if this works
void bitmap_save_ascii(Bitmap *bmp, const char *filename) {
  FILE *savedfile;
  savedfile = fopen(filename, "w");
  for (int h = 0; h < bmp->height; h++) {
    for (size_t w = 0; w < bmp->width; w++) {
      bool byte = bitmap_get_pixel(bmp, w, h);
      if (byte) {
        fprintf(savedfile, "#");
      } else {
        fprintf(savedfile, ".");
      }
    }
    fprintf(savedfile, "\n");
  }
  fclose(savedfile);
}
/* TODO:*/
void bitmap_save_pbm(Bitmap *bmp, const char *filename) {
  int bitsize = 0;
  FILE *fb = fopen(filename, "wb");
  bitsize = bmp->height * bmp->width;
}
