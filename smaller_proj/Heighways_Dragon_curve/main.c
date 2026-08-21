#include "build_the_dragon.h"
#include "render_the_dragon.h"
#include <stdlib.h>

int main(void) {
  char *dragon = create_l_syst(12);
  if (render_to_svg(dragon, "dragon.svg", 12)) {
    printf("dragon was created successfully in the .svg file\n");
  }
  free(dragon);
  return 0;
}
