#include "color.h"

color color_create_rgb(int r, int g, int b)
{
  color c;

  if (r == (r & 0xff) && g == (g & 0xff) && b == (b & 0xff))
    {
      c = r << 16 | g << 8 | b;
    }
  else
    {
      fprintf(stderr, "color_create_rgb(): Illegal RGB values: r = (0x%x), g = (0x%x), b = (0x%x)\n", r, g, b);
      c = DEF_ERROR_COLOR;
    }

  return c;
}

color color_add(color a, color b)
{
  return color_create_rgb(add_max_255(color_get_red(a), color_get_red(b)),
			  add_max_255(color_get_green(a), color_get_green(b)),
			  add_max_255(color_get_blue(a), color_get_blue(b)));
}

color color_scale(color c, real s)
{
  return color_create_rgb(color_get_red(c) * s, color_get_green(c) * s, color_get_blue(c) * s);
}

color color_multiply(color a, color b)
{
  return color_create_rgb((color_get_red(a) * color_get_red(b)) / 255,
			  (color_get_green(a) * color_get_green(b)) / 255,
			  (color_get_blue(a) * color_get_blue(b)) / 255);
}


int color_compare(color a, color b)
{
  return color_get_red(a) == color_get_red(b) &&
    color_get_green(a) == color_get_green(b) &&
    color_get_blue(a) == color_get_blue(b);
}

void color_print(FILE *os, color col)
{
  fprintf(os, "0x%02x%02x%02x", color_get_red(col), color_get_green(col), color_get_blue(col));
}
