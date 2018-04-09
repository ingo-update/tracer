#include "real.h"

#ifndef _COLOR_H_
#define _COLOR_H_

typedef struct color color;

struct color {
  unsigned char r,g,b;
};

color color_create_rgb(int r, int g, int b);
color color_add(color c1, color c2);
// color color_invert(color col);
color color_scale(color col, real s);
color color_multiply(color a, color b);

int color_get_red (color col);
int color_get_green (color col);
int color_get_blue (color col);

int color_compare(color a, color b);
void color_print(color col);

#endif /* _COLOR_H_ */