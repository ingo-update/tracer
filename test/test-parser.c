#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "color.h"
#include "world.h"
#include "camera.h"
#include "surface.h"

#include "parser.h"

extern int yyparse();
extern FILE *yyin;

world the_world;
camera the_camera;

int test_parse()
{
  int r, fail;

  fail = 0;
  the_world = world_create();

  yyin = fopen("examples/test.pov", "r");
  if (NULL == yyin)
    {
      fprintf(stderr, "Error, could not open test file 'examples/test.pov'\n");
      ++fail;
    }
  else
    {
      r = yyparse();
      if (r != 0)
	{
	  ++fail;
	  fprintf(stderr, "Error, yyparse returned %d\n", r);
	}
    }

  return fail;
}

int test_world()
{
  int no, nl, fail;
  struct list *lst;

  fail = 0;

  /* Check camera */

  if (0 == vector_compare(camera_get_direction(the_camera), vector_create(0,0,0)))
    {
      ++fail;
      fprintf(stderr, "Failure: Wrong camera direction: ");
      vector_print(stderr, camera_get_direction(the_camera));
      fprintf(stderr, "\n");
    }
  if (0 == vector_compare(camera_get_position(the_camera), vector_create(0,0,-1200)))
    {
      ++fail;
      fprintf(stderr, "Failure: Wrong camera position: ");
      vector_print(stderr, camera_get_position(the_camera));
      fprintf(stderr, "\n");
    }
  if (0 == vector_compare(camera_get_up(the_camera), vector_create(0,1,0)))
    {
      ++fail;
      fprintf(stderr, "Failure: Wrong camera up vector: ");
      vector_print(stderr, camera_get_up(the_camera));
      fprintf(stderr, "\n");
    }

  /* Count objects and light sources */

  no = nl = 0;
  for (lst = the_world->objects ; lst ; lst = lst->cdr) ++no;
  for (lst = the_world->lights ; lst ; lst = lst->cdr) ++nl;

  if (11 != no)
    {
      ++fail;
      fprintf(stderr, "Failure: found %d objects, should be 11.\n", no);
    }

  if (2 != nl)
    {
      ++fail;
      fprintf(stderr, "Failure: found %d light sources, should be 2.\n", nl);
    }

  return fail;
}

int test_image_maps()
{
  int fail;
  surface surf;
  struct list *lst;

  fail = 0;

  /* Check that the first sphere is solid color */
  surf = sphere_get_surface((sphere) the_world->objects->car);
  if (Color == surface_get_mode(surf))
    {
      ++fail;
      fprintf(stderr, "Failure: First sphere surface mode is %d, should be %d.\n", surface_get_mode(surf), TextureMap);
    }

  if (NULL != surf->texture)
    {
      ++fail;
      fprintf(stderr, "Failure: First sphere has image map loaded.\n");
    }

  /* Find the image for the triangle */
  for (lst = the_world->objects ; (((object) lst->car)->type != Triangle) ; lst = lst->cdr);
  surf = triangle_get_surface((triangle) lst->car);

  if (TextureMap != surface_get_mode(surf))
    {
      ++fail;
      fprintf(stderr, "Failure: Triangle surface mode is %d, should be %d.\n", surface_get_mode(surf), TextureMap);
    }

  if (NULL == surf->texture)
    {
      ++fail;
      fprintf(stderr, "Failure: Triangle image map was not loaded.\n");
    }

  return fail;
}

int main()
{
  int fail;

  fail = 0;

  fail += test_parse();
  fail += test_world();
  fail += test_image_maps();

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
