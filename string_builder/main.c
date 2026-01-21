#include "string_builder.h"

int main() {
  // test create/destroy and append funciton:
  Stringbuilder *sb = create_sb(10);
  append_sb(sb, "tes");
  append_sb(sb, "t");
  printf("so this is what our char currently looks: %s \n", sb->data);
#if 0
  for (int i = 0; i < 3; i++) {
    append_sb(sb, "test");
  }
  printf("so this is what our char currently looks: %s \n", sb->data);
#endif
  destroy_sb(sb);
  return 1;
}
