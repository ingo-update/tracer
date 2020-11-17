#include <stdio.h>

#include "color.h"

color color_create_rgb(int r, int g, int b)
{
  color c;

  if (r == (r & 0xff) && g == (g & 0xff) && b == (b & 0xff))
    {
      c.r = r; c.g = g; c.b = b;
    }
  else
    {
      fprintf(stderr, "color_create_rgb(): Illegal RGB values: r = (0x%x), g = (0x%x), b = (0x%x)\n", r, g, b);
      c.r = 0xff; c.g = 0x0; c.b = 0xff;
    }

  return c;
}

color color_add(color c1, color c2)
{
  return color_create_rgb(add_max_255(color_get_red(c1), color_get_red(c2)),
			  add_max_255(color_get_green(c1), color_get_green(c2)),
			  add_max_255(color_get_blue(c1), color_get_blue(c2)));
}

color color_scale(color col, real s)
{
  return color_create_rgb(color_get_red(col) * s, color_get_green(col) * s, color_get_blue(col) * s);
}

color color_multiply(color a, color b)
{
  return color_create_rgb((color_get_red(a) * color_get_red(b)) / 255,
			  (color_get_green(a) * color_get_green(b)) / 255,
			  (color_get_blue(a) * color_get_blue(b)) / 255);
}

int color_get_red (color col)
{
  return col.r;
}

int color_get_green (color col)
{
  return col.g;
}

int color_get_blue (color col)
{
  return col.b;
}


int color_compare(color a, color b)
{
  return (color_get_red(a) == color_get_red(b) &&
	  color_get_green(a) == color_get_green(b) &&
	  color_get_blue(a) == color_get_blue(b));
}


void color_print(FILE *os, color col)
{
  fprintf(os, "0x%02x%02x%02x", color_get_red(col), color_get_green(col), color_get_blue(col));
}
