#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"

#define ASC_FILENAME "build/test/test_asc.ppm"
#define BIN_FILENAME "build/test/test_bin.ppm"
#define ASC2BIN_FILENAME "build/test/test_asc2bin.ppm"
#define BIN2ASC_FILENAME "build/test/test_bin2asc.ppm"
#define CHECKERS_FILENAME "build/test/checkers.ppm"

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
      fprintf(stderr,"FAIL: Could read nonexisting bitmap\n");
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

  bm = bitmap_create(100,-00);
  if (NULL == bm)
    {
      fprintf(stderr,"ERROR: Test bitmap was not created.\n");
    }
  else
    {
      result = bitmap_write_ppm(bm, Binary, "/bad/path/to/file.ppm", "no comment");

      bitmap_destroy(bm);
      if (NULL != bm->pixels)
	{
	  ++fail;
	  fprintf(stderr,"FAIL: bitmap not destroyed properly.\n");
	}
    }

  bitmap_destroy(bm); // Should not cause a crash!

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

int main()
{
  int fail;
  create_files();

  fprintf(stdout, "test_bitmap: ");
  fail = 0;

  fail += test_errors();
  fail += compare_files();

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
