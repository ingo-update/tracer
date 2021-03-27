#include <stdio.h>

#include "defaults.h"
#include "types.h"

#ifndef _COLOR_H_
#define _COLOR_H_

#define add_max_255(a, b) (a + b > 255 ? 255 : a + b)

#define color_get_red(col) ((unsigned char) (col >> 16 & 0xff))
#define color_get_green(col) ((unsigned char) (col >> 8 & 0xff))
#define color_get_blue(col) ((unsigned char) (col & 0xff))

typedef unsigned int color;

color color_create_rgb(int r, int g, int b);
color color_add(color a, color b);
color color_scale(color col, real s);
color color_multiply(color a, color b);

int color_compare(color a, color b);
void color_print(FILE *os, color col);

#endif /* _COLOR_H_ */
