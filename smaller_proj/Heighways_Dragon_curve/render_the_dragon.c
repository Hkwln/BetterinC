#include "render_the_dragon.h"
#include "math.h"

bool render_to_svg(const char *lstring, const char *filename,
                   size_t iterations) {
  FILE *f = fopen(filename, "w");
  if (!f)
    return false;
  double x = 400.0, y = 750.0;
  double angle = 0.0;                             // 0 radians = pointing right
  double step = 600.0 / pow(2, iterations / 2.0); // Auto-scale to fit screen
  double angle_step = 3.14159265 / 2.0;           // 90 degrees in radians

  // Write SVG header
  fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"-110 -300 "
             "910 1440\" width=\"800\" height=\"800\">");
  fprintf(f, "<polyline points=\"");
  // parse x and y
  fprintf(f, "%f,%f ", x, y);
  for (const char *p = lstring; *p; p++) {
    if (*p == 'f' || *p == 'h') {
      // Move forward
      x += step * cos(angle);
      y -= step * sin(angle); // Subtract because SVG Y-axis is flipped
      fprintf(f, "%f,%f ", x, y);
    } else if (*p == '-') {
      angle -= angle_step; // Turn Right
    } else if (*p == '+') {
      angle += angle_step; // Turn Left
    }
    // Ignore any other characters (like spaces)
  }
  fprintf(f, "\" fill=\"none\" stroke=\"#2c3e50\" stroke-width=\"2\"/>\n");
  fprintf(f, "</svg>\n");
  fclose(f);

  return true;
}
