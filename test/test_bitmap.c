#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"

char *program_name = "";

void create_files()
{
  bitmap bm;
  int x,y;

  bm = bitmap_create(256,256);

  for (y = 0 ; y < 256 ; ++y)
    {
      for (x = 0 ; x < 256 ; ++x)
	{
	  bitmap_put_pixel(bm, x, y, color_create_rgb(x, y, (x + y) % 256));
	}
    }

  bitmap_write_ppm(bm, Ascii, "test_asc.ppm");
  bitmap_write_ppm(bm, Binary, "test_bin.ppm");
}

int test_errors()
{
  int fail;

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

  if (NULL != bitmap_read_ppm("../../Makefile"))
    {
      ++fail;
      fprintf(stderr,"FAIL: Could read Makefile as ppm\n");
    }

  return fail;
}

int compare_files()
{
  bitmap bma, bmb;
  int fail;
  int x, y;
  color ca, cb;

  fail = 0;

  bma = bitmap_read_ppm("test_asc.ppm");
  bmb = bitmap_read_ppm("test_bin.ppm");

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

  bitmap_write_ppm(bma, Binary, "test_asc2bin.ppm");
  bitmap_write_ppm(bmb, Ascii, "test_bin2asc.ppm");

  return fail;
}

int main()
{
  int fail;
  create_files();

  fail = 0;

  if (0 == fail)
    {
      fail += test_errors();
    }

  if (0 == fail)
    {
      fail += compare_files();
    }

  if (0 == fail)
    {
      fprintf(stderr, "SUCCESS\n");
      exit(EXIT_SUCCESS);
    }
  else
    {
      fprintf(stderr, "FAILURE\n");
      exit(EXIT_FAILURE);
    }
}
