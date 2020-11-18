#include "color.h"

#ifndef	_BITMAP_H_
#define	_BITMAP_H_

#define DEPTH 255 // only supported color depth

/* Supported PPM format */
#define COOKIE_ASC '3'
#define COOKIE_BIN '6'
#define FORMAT_ASC "%d %d %d\n"
#define FORMAT_BIN "%c%c%c"

typedef struct bitmap_t* bitmap;
struct bitmap_t
{
  int height, width;
  color *pixels;
};

typedef enum {Ascii=0, Binary=1} ppmtype;

bitmap bitmap_create (unsigned int width, unsigned int height);
bitmap bitmap_read_ppm(char *file_name);
void bitmap_destroy (bitmap bmp);

unsigned int bitmap_width(bitmap bmp);
unsigned int bitmap_height(bitmap bmp);
color bitmap_get_pixel(bitmap bmp, unsigned int x, unsigned int y);

void bitmap_put_pixel(bitmap bmp, unsigned int x, unsigned int y, color col);

int bitmap_write_ppm(bitmap bmp, ppmtype ptype, char *filename, char *comment);

#endif	/* _BITMAP_H_ */
