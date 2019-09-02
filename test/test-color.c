#include <stdio.h>
#include <stdlib.h>

#include "color.h"

int test_datatype()
{
  int fail;
  color c1;

  fail = 0;

  c1 = color_create_rgb(0x12, 0x111, 0xff);
  if (color_get_green(c1) == 0x111)
    {
      fprintf(stderr, "FAIL: Could create color with illegal RGB value.\n");
      ++fail;
    }

  fprintf(stderr, "  Color ");
  color_print(stderr, c1);
  fprintf(stderr, " created.\n");

  return fail;
}

int main()
{
  int fail = 0;

  if (!fail)
    {
      fail += test_datatype();
    }

  return fail;
}
