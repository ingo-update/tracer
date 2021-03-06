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
  object o;
  plane p;
  sphere s;

  fail = 0;

  /* Check that the first object in the list is a plane */
  o = the_world->objects->car;
  if (Plane != object_get_type(o))
    {
      ++fail;
      fprintf(stderr, "Failure: the first object is of type %d, should be %d\n", object_get_type(o), Plane);
    }

  /* Check that the plane is solid color */
  p = the_world->objects->car;
  surf = plane_get_surface(p);
  if (Color != surface_get_mode(surf))
    {
      ++fail;
      fprintf(stderr, "Failure: Plane surface mode is %d, should be %d.\n", surface_get_mode(surf), Color);
    }

  if (NULL != surf->texture)
    {
      ++fail;
      fprintf(stderr, "Failure: plane has image map loaded.\n");
    }

  /* Find the image for the triangle */
  for (lst = the_world->objects ; object_get_type(((object) (lst->car))) != Triangle ; lst = lst->cdr);
  p = lst->car;
  surf = plane_get_surface(p);
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

  /* Find a sphere with an image map */
  for (lst = the_world->objects ; lst ; lst = lst->cdr)
    if (object_get_type(((object) (lst->car))) == Sphere)
      {
	s = lst->car;
	if (TextureMap == surface_get_mode(sphere_get_surface(s))) break;
      }

  if (NULL == lst)
    {
      ++fail;
      fprintf(stderr, "Failure: No texture mapped image was found.\n");
    }

  return fail;
}

int main()
{
  int fail;

  fprintf(stdout, "test-parser: ");
  fail = 0;

  fail += test_parse();
  fail += test_world();
  fail += test_image_maps();

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
