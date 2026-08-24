#include "render_the_dragon.h"
#include "build_the_dragon.h"
#include "math.h"
#include <string.h>
void con_value(char *batch, double x, double y) {
  char *tmp[30];
  double var = x;
  for (size_t i = 0; var != 0; i++) {
  }
  strcat(batch, tmp);
}
bool render_to_svg(size_t n_generations, const char *filename) {
  if (n_generations == 0 || n_generations > 62)
    return false;

  FILE *f = fopen(filename, "w");
  if (!f)
    return false;

  const size_t n_segments = 1ULL << n_generations;
  double x = 400.0, y = 750.0;
  double angle = 0.0; // 0 radians = pointing right
  double step = 600.0 / pow(2, n_generations / 2.0); // Auto-scale to fit screen
  const double angle_step = 3.14159265 / 2.0;        // 90 degrees in radians

  // Write SVG header
  fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"-110 -300 "
             "910 1440\" width=\"800\" height=\"800\">");
  fprintf(f, "<polyline points=\"");
  fprintf(f, "%f,%f ", x, y);
  for (size_t i = 0; i < n_segments; i++) {
    // Move forward one segment
    x += step * cos(angle);
    y -= step * sin(angle); // Subtract because SVG Y-axis is flipped
    fprintf(f, "%f,%f ", x, y);
    // Turn after the segment: the dragon's turns are the paperfolding
    // sequence, so the next direction is known without any string.
    angle += (dragon_turn_at(i) == '+') ? angle_step : -angle_step;
  }
  fprintf(f, "\" fill=\"none\" stroke=\"#2c3e50\" stroke-width=\"2\"/>\n");
  fprintf(f, "</svg>\n");
  fclose(f);

  return true;
}
