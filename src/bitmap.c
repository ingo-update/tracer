#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "bitmap.h"

#define INSIDE(bmp, x, y) (x < bmp->width && y < bmp->height)

static unsigned int _get_ascii_value(FILE *infile)
{
  int found;
  unsigned int a;
  char c;

  found = 0;
  while (!found)
    {
      c = fgetc(infile);

      if ('#' == c)
	{
	  while ('\n' != c)
	    {
	      c = fgetc(infile);
	    }
	}

      if (c >= '0' && c <= '9')
	{
	  found = 1;
	}
    }

  a = 0;
  while ('0' <= c && '9' >= c)
    {
      a = a * 10 + (c - '0' );
      c = fgetc(infile);
    }

  ungetc(c,infile);

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
      fprintf(stderr, "bitmap_get_pixel(): Pixel (%dx%d) outside bitmap (%dx%d).\n", x, y, bmp->width, bmp->height);
      exit(EXIT_FAILURE);
    }
  return (bmp->pixels)[(y * bmp->width) + x];
}


void bitmap_put_pixel(bitmap bmp, unsigned int x, unsigned int y, color c)
{
  if (INSIDE(bmp, x, y))
    {
      bmp->pixels[(y * bmp->width) + x] = c;
    }
  else
    {
      fprintf(stderr, "bitmap_put_pixel(): Pixel (%dx%d) outside bitmap (%dx%d).\n",
	      x, y, bmp->width, bmp->height);
      exit(EXIT_FAILURE);
    }
}

void bitmap_write_ppm(bitmap bmp, ppmtype ptype, char *filename, char *comment)
{
  FILE *outfile;
  char *cookie, *format;
  unsigned int x,y;
  color p;

  if (NULL == filename)
    {
      outfile = stdout;
    }
  else
    {
      outfile = fopen(filename,"w");
      if (NULL == outfile)
	{
	  fprintf(stderr,"bitmap_write_ppm(): Could not open file '%s'\n", filename);
	  exit(EXIT_FAILURE);
	}
    }

  if (Ascii == ptype)
    {
      cookie = "P3";
      format = "%d %d %d\n";
    }
  else
    {
      cookie = "P6";
      format = "%c%c%c";
    }

  // Header
  fprintf(outfile, "%s\n", cookie);
  fprintf(outfile, "#%s\n", comment);
  fprintf(outfile, "%d %d %d\n",bmp->width, bmp->height, DEPTH);

  // Pixel data
  for (y = 0 ; y < bmp->height ; y++)
    {
      for (x = 0 ; x < bmp->width ; x++)
	{
	  p = bitmap_get_pixel(bmp, x, y);
	  fprintf(outfile, format, color_get_red(p), color_get_green(p), color_get_blue(p));
	}
    }
  fclose(outfile);
}

bitmap bitmap_read_ppm(char *filename)
{
  FILE *infile;
  bitmap bmp;
  ppmtype ptype;
  unsigned int width, height, depth;
  unsigned int r,g,b,x,y;
  unsigned char c,d;

  if (NULL == filename)
    {
      infile = stdin;
    }
  else
    {
      infile = fopen(filename,"r");
      if (NULL == infile)
	{
	  fprintf(stderr,"bitmap_read_ppm(): Could not open file '%s'\n", filename);
	  return NULL;
	}
    }

  c = fgetc(infile);
  d = fgetc(infile);

  if (c == 'P')
    {
      if (d == '3')
	{
	  ptype = Ascii;
	}
      else if (d == '6')
	{
	  ptype = Binary;
	}
      else
	{
	  fprintf(stderr,"bitmap_read_ppm(): Unsupported ppm type 'P%c'\n", d);
	  return NULL;
	}
    }
  else
    {
      fprintf(stderr,"bitmap_read_ppm(): File '%s' is not PPM (%c%c)\n", filename, c, d);
      return NULL;
    }

  width = _get_ascii_value(infile);
  height = _get_ascii_value(infile);
  depth = _get_ascii_value(infile);

  if (DEPTH != depth)
    {
      fprintf(stderr,"bitmap_read_ppm(): Unsupported depth %d\n", depth);
      return NULL;
    }

  bmp = bitmap_create(width, height);

  if (Ascii == ptype)
    {
      for (y = 0 ; y < height ; ++y)
	{
	  for (x = 0 ; x < width ; ++x)
	    {
	      r = _get_ascii_value(infile);
	      g = _get_ascii_value(infile);
	      b = _get_ascii_value(infile);

	      if (feof(infile))
		{
		  fprintf(stderr,"bitmap_read_ppm: Unexpected EOF after %d pixels\n", (y * width + x));
		  bitmap_destroy(bmp);
		  return NULL;
		}
	      bmp->pixels[y * width + x] = color_create_rgb(r, g, b);
	    }
	}
    }
  else
    {
      // Remove single byte before starting
      c = fgetc(infile);

      for (y = 0 ; y < height ; ++y)
	{
	  for (x = 0 ; x < width ; ++x)
	    {
	      r = fgetc(infile);
	      g = fgetc(infile);
	      b = fgetc(infile);

	      if (feof(infile))
		{
		  fprintf(stderr,"bitmap_read_ppm: Unexpected EOF after %d pixels\n", (y * width + x));
		  bitmap_destroy(bmp);
		  return NULL;
		}
	      bmp->pixels[y * width + x] = color_create_rgb(r, g, b);
	    }
	}
    }

  return bmp;
}
