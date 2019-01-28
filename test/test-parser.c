#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "color.h"
#include "world.h"
#include "camera.h"

#include "parser.h"

world the_world;
camera the_camera;

int test_parse()
{
  return 0;
}

int main()
{
  int fail;

  fail = 0;

  fail += test_parse();

    if (0 == fail)
    {
      fprintf(stdout, "SUCCESS\n");
      exit(EXIT_SUCCESS);
    }
  else
    {
      fprintf(stdout, "FAILURE\n");
      exit(EXIT_FAILURE);
    }
}
