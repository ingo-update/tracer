#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "color.h"
#include "world.h"
#include "camera.h"

#include "parser.h"

extern int yyparse();
extern FILE *yyin;

world the_world;
camera the_camera;

int test_parse()
{
  int r;
  int fail;

  fail = 0;
  the_world = world_create();

  yyin = fopen("examples/test.pov", "r");
  if (NULL == yyin)
    {
      fprintf(stderr, "Failure, could not open test file 'examples/test.pov'\n");
      ++fail;
    }
  else
    {
      r = yyparse();
      if (r != 0)
	{
	  ++fail;
	  fprintf(stderr, "Failure, yyparse returned %d\n", r);
	}
      else
	{
	  /* Remove this! */
	  fprintf(stderr, "Test parsing succeeded!\n");
	}
    }
  return fail;
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
