#include "draw.h"
#include "bitmap.h"
#include <stdlib.h>
#include <time.h>

// x and y are the starting point width and height are the size of the rectangle
void bitmap_fill_rect(Bitmap *bmp, uint32_t x, uint32_t y, uint32_t width,
                      uint32_t height, bool white) {
  for (int i = x; i < x + width; i++) {
    for (int b = y; b < y + height; b++) {
      bitmap_set_pixel(bmp, i, b, white);
    }
  }
}

void bitmap_draw_hline(Bitmap *bmp, uint32_t x, uint32_t y, uint32_t length,
                       bool white) {
  for (int i = x; i < x + length; i++) {
    bitmap_set_pixel(bmp, i, y, white);
  }
}

// Fisher-Yates shuffle
static void shuffle_array(int *array, int n) {
  for (int i = n - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

// Box Animation initialisieren
BoxAnimation box_anim_init(int x, int y, int width, int height) {
  BoxAnimation anim = {
    .x = x,
    .y = y, 
    .width = width,
    .height = height,
    .progress = 0,
    .complete = false,
    .drawn_pixels = 0
  };
  
  // Berechne totale Pixel im Rahmen
  anim.total_pixels = (width * 2) + (height * 2) - 4;  // Ecken nur einmal
  
  // Erstelle Array mit allen Pixel-Positionen
  anim.draw_order = malloc(anim.total_pixels * sizeof(int));
  
  int idx = 0;
  
  // Obere Linie (links nach rechts)
  for (int i = 0; i < width; i++) {
    anim.draw_order[idx++] = i;  // x-offset, y=0
  }
  
  // Rechte Linie (oben nach unten, ohne obere Ecke)
  for (int i = 1; i < height; i++) {
    anim.draw_order[idx++] = width - 1 + (i * 1000);  // kodiert als x + y*1000
  }
  
  // Untere Linie (rechts nach links, ohne rechte Ecke)
  for (int i = width - 2; i >= 0; i--) {
    anim.draw_order[idx++] = i + ((height - 1) * 1000);
  }
  
  // Linke Linie (unten nach oben, ohne beide Ecken)
  for (int i = height - 2; i > 0; i--) {
    anim.draw_order[idx++] = 0 + (i * 1000);
  }
  
  // Random shuffle
  srand(time(NULL) + x + y);  // Seed mit Position für Varianz
  shuffle_array(anim.draw_order, anim.total_pixels);
  
  return anim;
}

// Ein Frame der Box-Animation zeichnen
bool box_anim_draw_frame(BoxAnimation *anim, Bitmap *bmp, int speed) {
  if (anim->complete) return true;
  
  // Zeichne 'speed' Pixel pro Frame
  int pixels_to_draw = speed;
  
  for (int i = 0; i < pixels_to_draw && anim->drawn_pixels < anim->total_pixels; i++) {
    int encoded = anim->draw_order[anim->drawn_pixels];
    int px = encoded % 1000;
    int py = encoded / 1000;
    
    int actual_x = anim->x + px;
    int actual_y = anim->y + py;
    
    if (actual_x >= 0 && actual_x < bmp->width && actual_y >= 0 && actual_y < bmp->height) {
      bitmap_set_pixel(bmp, actual_x, actual_y, 1);
    }
    
    anim->drawn_pixels++;
  }
  
  anim->progress = (anim->drawn_pixels * 100) / anim->total_pixels;
  
  if (anim->drawn_pixels >= anim->total_pixels) {
    anim->complete = true;
    anim->progress = 100;
  }
  
  return anim->complete;
}

// Box-Animation aufräumen
void box_anim_free(BoxAnimation *anim) {
  if (anim->draw_order) {
    free(anim->draw_order);
    anim->draw_order = NULL;
  }
}

// Box komplett zeichnen mit Unicode-Zeichen (für Terminal-Output)
void draw_box_unicode(int x, int y, int width, int height) {
  // Ecken
  printf("\033[%d;%dH┌", y, x);                          // Oben links
  printf("\033[%d;%dH┐", y, x + width - 1);              // Oben rechts
  printf("\033[%d;%dH└", y + height - 1, x);             // Unten links
  printf("\033[%d;%dH┘", y + height - 1, x + width - 1); // Unten rechts

  // Horizontale Linien
  for (int i = 1; i < width - 1; i++) {
    printf("\033[%d;%dH─", y, x + i);              // Oben
    printf("\033[%d;%dH─", y + height - 1, x + i); // Unten
  }

  // Vertikale Linien
  for (int i = 1; i < height - 1; i++) {
    printf("\033[%d;%dH│", y + i, x);             // Links
    printf("\033[%d;%dH│", y + i, x + width - 1); // Rechts
  }
}

// Box komplett zeichnen auf Bitmap (ohne Animation)
void bitmap_draw_box(Bitmap *bmp, int x, int y, int width, int height) {
  // Obere und untere Linie
  for (int i = 0; i < width; i++) {
    if (x + i < bmp->width && y < bmp->height)
      bitmap_set_pixel(bmp, x + i, y, 1);
    if (x + i < bmp->width && y + height - 1 < bmp->height)
      bitmap_set_pixel(bmp, x + i, y + height - 1, 1);
  }
  
  // Linke und rechte Linie
  for (int i = 0; i < height; i++) {
    if (x < bmp->width && y + i < bmp->height)
      bitmap_set_pixel(bmp, x, y + i, 1);
    if (x + width - 1 < bmp->width && y + i < bmp->height)
      bitmap_set_pixel(bmp, x + width - 1, y + i, 1);
  }
}
