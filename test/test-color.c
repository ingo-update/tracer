#include <stdio.h>
#include <stdlib.h>

#include "color.h"

int test_datatype()
{
  int fail;
  color c1, c2;

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

  c2 = color_create_rgb(0x17, 0x42, 0xbf);
  if (color_get_red(c2) != 0x17)
    {
      fprintf(stderr, "FAIL: color_get_red() returned %x, should be %x\n", color_get_red(c2), 0x17);
      ++fail;
    }
  if (color_get_green(c2) != 0x42)
    {
      fprintf(stderr, "FAIL: color_get_green() returned %x, should be %x\n", color_get_green(c2), 0x42);
      ++fail;
    }
  if (color_get_blue(c2) != 0xbf)
    {
      fprintf(stderr, "FAIL: color_get_blue() returned %x, should be %x\n", color_get_blue(c2), 0xbf);
      ++fail;
    }

  return fail;
}

int main()
{
  int fail = 0;

  if (!fail)
    {
      fail += test_datatype();
    }

    if (0 == fail)
    {
      fprintf(stdout, "SUCCESS\n");
      exit(EXIT_SUCCESS);
    }
  else
    {
      fprintf(stdout, "FAILURE; %d tests failed.\n", fail);
      exit(EXIT_FAILURE);
    }
}
