#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "bitmap.h"

#define ASC_FILENAME "build/test/test_asc.ppm"
#define BIN_FILENAME "build/test/test_bin.ppm"
#define ASC2BIN_FILENAME "build/test/test_asc2bin.ppm"
#define BIN2ASC_FILENAME "build/test/test_bin2asc.ppm"
#define CHECKERS_FILENAME "build/test/checkers.ppm"

#define COLOR_BLACK color_create_rgb(0, 0, 0)

void create_files()
{
  bitmap bm;
  int x, y;
  color c1, c2, c;

  bm = bitmap_create(256,256);

  for (y = 0 ; y < 256 ; ++y)
    {
      for (x = 0 ; x < 256 ; ++x)
	{
	  bitmap_put_pixel(bm, x, y, color_create_rgb(x, y, (x + y) % 256));
	}
    }

  bitmap_write_ppm(bm, Ascii, ASC_FILENAME, "ASCII test file");
  bitmap_write_ppm(bm, Binary, BIN_FILENAME, "Binary test file");

  bitmap_destroy(bm);

  c1 = color_create_rgb(255, 255, 255);
  c2 = color_create_rgb(255, 0, 0);

  bm = bitmap_create(128, 64);

  for (y = 0 ; y < 64 ; ++y)
    {
      for (x = 0 ; x < 128 ; ++x)
	{
	  if (y & 4) c = x & 4 ? c1 : c2;
	  else c = x & 4 ? c2 : c1;
	  bitmap_put_pixel(bm, x, y, c);
	}
    }

  bitmap_write_ppm(bm, Binary, CHECKERS_FILENAME, "Checkers");
}

int test_errors()
{
  int fail, result;
  bitmap bm;

  fail = 0;
  if (NULL != bitmap_create(0,0))
    {
      ++fail;
      fprintf(stderr,"FAIL: Could create size 0 bitmap\n");
    }

  if (NULL != bitmap_read_ppm("nonexisting.ppm"))
    {
      ++fail;
      fprintf(stderr,"FAIL: Could read nonexisting bitmap file\n");
    }

  if (NULL != bitmap_read_ppm("Makefile"))
    {
      ++fail;
      fprintf(stderr,"FAIL: Could read Makefile as ppm\n");
    }

  if (NULL != bitmap_create(100,-100))
    {
      ++fail;
      fprintf(stderr,"FAIL: Could create bitmap with negative size\n");
    }

  bm = bitmap_create(100,100);
  if (NULL == bm)
    {
      fprintf(stderr,"ERROR: Test bitmap was not created\n");
    }
  else
    {
      result = bitmap_write_ppm(bm, Binary, "/bad/path/to/file.ppm", "no comment");
      if (result == 0)
	{
	  ++fail;
	  fprintf(stderr,"FAIL: No error when trying to save bitmap to illegal filename\n");
	}

      bitmap_destroy(bm);
      if (NULL != bm->pixels)
	{
	  ++fail;
	  fprintf(stderr,"FAIL: bitmap not destroyed properly\n");
	}
    }

  bitmap_destroy(bm); // Should cause error message but no crash!

  return fail;
}

int compare_files()
{
  bitmap bma, bmb;
  int fail;
  int x, y;
  color ca, cb;

  fail = 0;

  bma = bitmap_read_ppm(ASC_FILENAME);
  bmb = bitmap_read_ppm(BIN_FILENAME);

  if (bitmap_width(bma) != bitmap_width(bmb))
    {
      ++fail;
      fprintf(stderr,"FAIL: Bitmaps differ in width, %d != %d\n", bitmap_width(bma), bitmap_width(bmb));
    }

  if (bitmap_height(bma) != bitmap_height(bmb))
    {
      ++fail;
      fprintf(stderr,"FAIL: Bitmaps differ in height, %d != %d\n", bitmap_height(bma), bitmap_height(bmb));
    }

  for (y = 0 ; y < bitmap_height(bma) ; ++y)
    {
      for (x = 0 ; x < bitmap_width(bma) ; ++x)
	{
	  ca = bitmap_get_pixel(bma, x, y);
	  cb = bitmap_get_pixel(bmb, x, y);

	  if (0 == color_compare(ca, cb))
	    {
	      fprintf(stderr, "  Ascii  color at %d,%d is (%d %d %d)\n", x, y, color_get_red(ca), color_get_green(ca), color_get_blue(ca));
	      fprintf(stderr, "  Binary color at %d,%d is (%d %d %d)\n", x, y, color_get_red(cb), color_get_green(cb), color_get_blue(cb));
	      fprintf(stderr, "FAIL: Different colors at %d, %d\n", x, y);
	      ++fail;
	    }
	}
    }

  bitmap_write_ppm(bma, Binary, ASC2BIN_FILENAME, "ASCII to Binary test file");
  bitmap_write_ppm(bmb, Ascii, BIN2ASC_FILENAME, "Binary to ASCII test file");

  return fail;
}

static color _make_color(int a, int m)
{
  real c;
  int h, x;

  c = ((real) a / (real) m);
  h = (int) (c * 6);
  x = ((c * 6) - h) * 255;

  switch (h)
    {
    case 0:
      return color_create_rgb(255, x, 0);
    case 1:
      return color_create_rgb(x, 255, 0);
    case 2:
      return color_create_rgb(0, 255, x);
    case 3:
      return color_create_rgb(0, x, 255);
    case 4:
      return color_create_rgb(x, 0, 255);
    default:
      return color_create_rgb(256, 0, x);
    }
}

int test_mandelbrot()
{
  real ReCenter = -0.5;
  real ImCenter = 0;
  int xSize = 512;
  int ySize = 512;
  real ReSize = 2;
  real ImSize = 2;
  int m = 128;

  bitmap bm;
  int x, y, c;

  real ReC, ImC, ReZ, ImZ, t;
  real ReMin, ImMin, ReScale, ImScale;

  ReMin = ReSize / -2.0 + ReCenter;
  ImMin = ImSize / -2.0 + ImCenter;
  ReScale = ReSize / (real) xSize;
  ImScale = ImSize / (real) ySize;

  bm = bitmap_create(xSize, ySize);

  for (y = 0 ; y < ySize ; ++y)
    {
      ImC = y * ImScale + ImMin;
      for (x = 0 ; x < xSize ; ++x)
	{
	  ReC = x * ReScale + ReMin;

	  ReZ = ImZ = 0;
	  for (c = 0 ; c < m && (ReZ * ReZ + ImZ * ImZ) < 4 ; ++c)
	    {
	      t = ReZ * ReZ - ImZ * ImZ + ReC;
	      ImZ = 2 * ReZ * ImZ + ImC;
	      ReZ = t;
	    }

	  bitmap_put_pixel(bm, x, y, (c == m ? COLOR_BLACK : _make_color(c, m)));
	}
    }

  bitmap_write_ppm(bm, Binary, "build/test/mandelbrot.ppm", "Mandelbrot set");
  return 0;
}

int main()
{
  int fail;
  create_files();

  fprintf(stdout, "test-bitmap: ");
  fail = 0;

  fail += test_errors();
  fail += compare_files();
  fail += test_mandelbrot();

  if (0 == fail)
    {
      fprintf(stdout, "SUCCESS\n");
      return EXIT_SUCCESS;
    }
  else
    {
      fprintf(stdout, "FAILURE; %d tests failed.\n", fail);
      return EXIT_FAILURE;
    }
}
