#include <stdio.h>

#include "color.h"

color color_create_rgb(int r, int g, int b) {
  color c;

  c.r = r;
  c.g = g;
  c.b = b;

  return c;
}


color color_add(color c1, color c2) {
  color c;

  c.r = (c1.r + c2.r); if (c.r > 255) c.r = 255;
  c.g = (c1.g + c2.g); if (c.g > 255) c.g = 255;
  c.b = (c1.b + c2.b); if (c.b > 255) c.b = 255;

  return c;
}

/*
color color_invert(color col) {
  color c;

  c.r = 255-col.r;
  c.g = 255-col.g;
  c.b = 255-col.b;

  return c;
}
*/

color color_scale(color col, real s) {
  color c;

  c.r = s * col.r;
  c.g = s * col.g;
  c.b = s * col.b;

  return c;
}

color color_multiply(color a, color b) {
  color c;

  c.r = 255 * ((real) a.r / (real) 255) * ((real) b.r / (real) 255);
  c.g = 255 * ((real) a.g / (real) 255) * ((real) b.g / (real) 255);
  c.b = 255 * ((real) a.b / (real) 255) * ((real) b.b / (real) 255);

  return c;
}


int color_get_red (color col) {

  return col.r;
}


int color_get_green (color col) {

  return col.g;
}


int color_get_blue (color col) {

  return col.b;
}

int color_compare(color a, color b)
{
  return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
}
