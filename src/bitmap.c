#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"

#define INSIDE(bmp, x, y) (x < bmp->width && y < bmp->height)

static unsigned int _get_ascii_value(FILE *infile)
{
  int found;
  unsigned int a;
  char c;

  /* Find first digit while skipping comments (# to EOL) */
  found = 0;
  while (!found)
    {
      c = fgetc(infile);
      if ('#' == c) while ('\n' != c) c = fgetc(infile);
      if (c >= '0' && c <= '9') found = 1;
    }

  /* Read digits */
  a = 0;
  while ('0' <= c && '9' >= c)
    {
      a = a * 10 + (c - '0' );
      c = fgetc(infile);
    }

  ungetc(c, infile);

  return a;
}


bitmap bitmap_create(unsigned int w, unsigned int h)
{
  bitmap bmp;

  if ((1 > w) || (1 > h))
    {
      fprintf(stderr, "bitmap_create(): Illegal bitmap size (%d x %d)\n", w, h);
      return NULL;
    }

  bmp = malloc(sizeof(struct bitmap_t));
  if (NULL == bmp) {
    fprintf(stderr, "bitmap_create(): Could not allocate bitmap struct.\n");
    return NULL;
  }

  bmp->pixels = malloc(w * h * sizeof(color));
  if (NULL == bmp->pixels)
    {
      fprintf(stderr, "bitmap_create(): Could not allocate pixels.\n");
      return NULL;
    }

  bmp->width = w;
  bmp->height = h;

  return bmp;
}


void bitmap_destroy(bitmap bmp)
{
  if (NULL == bmp || NULL == bmp->pixels)
    {
      fprintf(stderr, "bitmap_destroy(): Bad bitmap.\n");
    }

  free(bmp->pixels);
  free(bmp);
}


unsigned int bitmap_width(bitmap bmp)
{
  if (NULL == bmp)
    {
      fprintf(stderr, "bitmap_width(): Bad bitmap.\n");
      return 0;
    }
  return bmp->width;
}


unsigned int bitmap_height(bitmap bmp)
{
  if (NULL==bmp)
    {
      fprintf(stderr, "bitmap_height(): Bad bitmap.\n");
      return 0;
    }
  return bmp->height;
}


color bitmap_get_pixel(bitmap bmp, unsigned int x, unsigned int y)
{
  if (!INSIDE(bmp, x, y))
    {
      fprintf(stderr, "bitmap_get_pixel(): Pixel (%d,%d) outside bitmap (%dx%d).\n", x, y, bmp->width, bmp->height);
      exit(EXIT_FAILURE);
    }
  return (bmp->pixels)[(y * bmp->width) + x];
}


void bitmap_put_pixel(bitmap bmp, unsigned int x, unsigned int y, color c)
{
  if (!INSIDE(bmp, x, y))
    {
      fprintf(stderr, "bitmap_put_pixel(): Pixel (%d,%d) outside bitmap (%dx%d).\n",
	      x, y, bmp->width, bmp->height);
      exit(EXIT_FAILURE);
    }

  bmp->pixels[(y * bmp->width) + x] = c;
}


int bitmap_write_ppm(bitmap bmp, ppmtype ptype, char *filename, char *comment)
{
  FILE *outfile;
  unsigned int x, y;
  color p;
  char cookie[] = {COOKIE_ASC, COOKIE_BIN}; // Magic cookie for Ascii and Binary.
  char *format[] = {FORMAT_ASC, FORMAT_BIN}; // Output format for pixels in Ascci and Binary.

  outfile = stdout;
  if (NULL != filename)
    {
      outfile = fopen(filename,"w");
      if (NULL == outfile)
	{
	  fprintf(stderr,"bitmap_write_ppm(): Could not open file '%s'\n", filename);
	  return -1;
	}
    }

  if ((ptype != Ascii) && (ptype != Binary))
    {
      fprintf(stderr,"bitmap_write_ppm(): Illegal ppm type %d\n", ptype);
      return -1;
    }

  // Header
  fprintf(outfile,"P%c\n#%s\n%d %d %d\n", cookie[ptype], comment, bmp->width, bmp->height, DEPTH);

  // Pixel data
  for (y = 0 ; y < bmp->height ; y++)
    {
      for (x = 0 ; x < bmp->width ; x++)
	{
	  p = bitmap_get_pixel(bmp, x, y);
	  fprintf(outfile, format[ptype], color_get_red(p), color_get_green(p), color_get_blue(p));
	}
    }
  fclose(outfile);

  return 0;
}

bitmap bitmap_read_ppm(char *filename)
{
  FILE *infile;
  bitmap bmp;
  ppmtype ptype;
  unsigned int width, height, depth;
  unsigned int x, y;
  unsigned char r, g, b;
  unsigned char c, d;

  infile = stdin;
  if (NULL != filename)
    {
      infile = fopen(filename,"r");
      if (NULL == infile)
	{
	  fprintf(stderr,"bitmap_read_ppm(): Could not open file '%s'\n", filename);
	  return NULL;
	}
    }

  // Try to read the magic cookie:
  c = fgetc(infile);
  d = fgetc(infile);

  if (c == 'P')
    switch (d)
      {
      case COOKIE_ASC:
	ptype = Ascii;
	break;
      case COOKIE_BIN:
	ptype = Binary;
	break;
      default:
	fprintf(stderr,"bitmap_read_ppm(): Unsupported ppm type 'P%c'\n", d);
	return NULL;
      }
  else
    {
      fprintf(stderr,"bitmap_read_ppm(): File '%s' is not PPM (%c%c)\n", filename, c, d);
      return NULL;
    }

  // Read bitmap information
  width = _get_ascii_value(infile);
  height = _get_ascii_value(infile);
  depth = _get_ascii_value(infile);

  c = fgetc(infile); // Remove one extra whitespace.

  if (DEPTH != depth)
    {
      fprintf(stderr,"bitmap_read_ppm(): Unsupported depth %d\n", depth);
      return NULL;
    }

  bmp = bitmap_create(width, height);

  for (y = 0 ; y < height ; ++y)
    for (x = 0 ; x < width ; ++x)
      {
	r = (Ascii == ptype) ? _get_ascii_value(infile) : fgetc(infile);
	g = (Ascii == ptype) ? _get_ascii_value(infile) : fgetc(infile);
	b = (Ascii == ptype) ? _get_ascii_value(infile) : fgetc(infile);

	if (feof(infile))
	  {
	    fprintf(stderr,"bitmap_read_ppm: Unexpected EOF after %d pixels\n", (y * width + x));
	    bitmap_destroy(bmp);
	    return NULL;
	  }
	bmp->pixels[y * width + x] = color_create_rgb(r, g, b);
      }

  return bmp;
}
