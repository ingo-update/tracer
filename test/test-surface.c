#include <stdio.h>
#include <stdlib.h>

#include "surface.h"

#define GOOD_TEXTURE_FILE "build/test/checkers.ppm"
#define BAD_TEXTURE_FILE "Makefile"

#define TEST_COLOR color_create_rgb(0x42, 0x17, 0x00)

int test_datatype()
{
  int fail;
  struct pigment p1, p2;
  struct finish f;
  surface s1, s2;

  fail = 0;

  p1.type = Color;
  p1.col = TEST_COLOR;

  p2.type = TextureMap;
  p2.texture_name = GOOD_TEXTURE_FILE;

  f.reflection = 0.2;
  f.diffuse = 0.5;

  s1 = surface_create(p1, f);
  if (NULL == s1)
    {
      fprintf(stdout, "FAIL: Could not create a color surface\n");
      ++fail;
    }
  else
    {
      if (Color != surface_get_mode(s1))
	{
	  fprintf(stdout, "FAIL: Color surface mode is %d, should be %d\n", surface_get_mode(s1), Color);
	  ++fail;
	}

      if (!color_compare(surface_get_color(s1), TEST_COLOR))
	{
	  fprintf(stdout, "FAIL: Color RGB is ");
	  color_print(stdout, surface_get_color(s1));
	  fprintf(stdout, ", should be");
	  color_print(stdout, TEST_COLOR);
	  fprintf(stdout, "\n");
	  ++fail;
	}

      if (NULL != surface_get_texture_map(s1))
	{
	  fprintf(stdout, "FAIL: Color surface should not have a bitmap\n");
	  ++fail;
	}
    }

  s2 = surface_create(p2, f);
  if (NULL == s2)
    {
      fprintf(stdout, "FAIL: Could not create a texture map surface with bitmap '%s'\n", GOOD_TEXTURE_FILE);
      ++fail;
    }
  else
    {
      if (TextureMap != surface_get_mode(s2))
	{
	  fprintf(stdout, "FAIL: Color surface mode is %d, should be %d\n", surface_get_mode(s2), TextureMap);
	  ++fail;
	}

      if (NULL == surface_get_texture_map(s2))
	{
	  fprintf(stdout, "FAIL: Texture map surface does not have a bitmap\n");
	  ++fail;
	}

      if (!color_compare(surface_get_color(s2), DEF_BG_COLOR))
	{
	  fprintf(stdout, "FAIL: Texture map surface returns color ");
	  color_print(stdout, surface_get_color(s2));
	  fprintf(stdout, ", should be default color ");
	  color_print(stdout, DEF_BG_COLOR);
	  fprintf(stdout, "\n");
	  ++fail;
	}
    }

  f.reflection = -0.3;
  s1 = surface_create(p1, f);
  if (NULL != s1)
    {
      fprintf(stdout, "FAIL: Could create surface with negative reflection\n");
      ++fail;
    }

  f.reflection = 0.2;
  f.diffuse = -0.5;
  s1 = surface_create(p1, f);
  if (NULL != s1)
    {
      fprintf(stdout, "FAIL: Could create surface with negative diffuse reflection\n");
      ++fail;
    }

  f.diffuse = 0.5;
  p2.texture_name = BAD_TEXTURE_FILE;
  s2 = surface_create(p2, f);
  if (NULL != s1)
    {
      fprintf(stdout, "FAIL: Could create texture map surface with non-image texture map file '%s'\n", BAD_TEXTURE_FILE);
      ++fail;
    }

  return fail;
}

int main()
{
  int fail = 0;

  if (!fail)
    {
      fail += test_datatype();
    }

    if (0 == fail)
    {
      fprintf(stdout, "SUCCESS\n");
      exit(EXIT_SUCCESS);
    }
  else
    {
      fprintf(stdout, "FAILURE; %d tests failed.\n", fail);
      exit(EXIT_FAILURE);
    }
}
