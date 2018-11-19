#include <stdio.h>
#include <stdlib.h>

#include "options.h"
#include "world.h"
#include "camera.h"
#include "bitmap.h"
#include "trace.h"

extern int yyparse();
extern FILE *yyin;

/* These must be global for the parser to access them. */
world the_world;
camera the_camera;

int main(int argc, char **argv)
{
  options opt;
  int res;
  bitmap bmp;

  opt = options_get_options(argc, argv);
  if (NULL == opt)
    {
      fprintf(stderr, "Illegal options\n");
      exit(EXIT_FAILURE);
    }

  if (NULL != options_get_in_file_name(opt))
    {
      yyin = fopen(options_get_in_file_name(opt), "r");
      if (NULL == yyin)
	{
	  fprintf(stderr, "Input file '%s' not found\n", options_get_in_file_name(opt));
	  exit(EXIT_FAILURE);
	}
    }

  the_world = world_create();
  res = yyparse();
  if (0 != res)
    {
      fprintf(stderr, "Syntax error in description\n");
      exit(EXIT_FAILURE);
    }

  bmp = trace(the_world, the_camera, opt);
  if (NULL == bmp)
    {
      fprintf(stderr, "Tracing failed\n");
      exit(EXIT_FAILURE);
    }

  bitmap_write_ppm(bmp, options_get_ppm_mode(opt), options_get_out_file_name(opt), options_get_comment(opt));
  return EXIT_SUCCESS;
}
