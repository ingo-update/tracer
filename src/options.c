#include <stdio.h>
#include <stdlib.h>

#include "defaults.h"
#include "options.h"

void _options_print_usage(char *exe)
{
  fputs("Usage: \n", stderr);
  fputs(exe, stderr);
  fputs(" opts\n", stderr);
  fputs("\n", stderr);
  fputs("Where opts are some of:\n", stderr);
  fputs("  -x  INTEGER  Width of output image in pixels\n", stderr);
  fputs("  -y  INTEGER  Height of output image in pixels\n", stderr);
  fputs("  -w  FLOAT    Width of output image in world coordinates\n", stderr);
  fputs("  -h  FLOAT    Height of output image in world coordinates\n", stderr);
  fputs("  -z  INTEGER  Zoom-value for camera (default = 1)\n", stderr);
  fputs("  -d  INTEGER  Reflection depth (default = 5)\n", stderr);
  fputs("  -i  STRING   Name of input file, default is to read from stdin\n", stderr);
  fputs("  -o  STRING   Name of output image file, default is to send data to stdout\n", stderr);
  fputs("  -c  STRING   Comment in produced image file\n", stderr);
  fputs("  -q           \'Quick mode\', no reflection or shading\n", stderr);
  fputs("  -?           Show this help text and exit\n", stderr);
  fputs("\n", stderr);
  fputs("At least 3 of {-x, -y, -w, -h} must be provided.\n", stderr);
}

options options_get_options(int argc, char **argv)
{
  int i, dim_f;
  options opt;

  opt = malloc(sizeof(struct options_t));
  opt->out_file_name = NULL;
  opt->in_file_name = NULL;

  opt->reflection_depth = DEF_REFLECTION_DEPTH;
  opt->zoom = DEF_ZOOM;

  dim_f = 0;
  for (i = 1 ; i < argc; ++i)
    {
      if (argv[i][0] != '-')
	{
	  fprintf(stderr, "Illegal option \'%s\'\n", argv[i]);
	}
      else
	{
	  switch(argv[i][1])
	    {
	    case 'i':
	      opt->in_file_name = ('\0' != argv[i][2]) ? &argv[i][2] : argv[++i];
	      break;
	    case 'o':
	      opt->out_file_name = ('\0' != argv[i][2]) ? &argv[i][2] : argv[++i];
	      break;
	    case 'c':
	      opt->comment = ('\0' != argv[i][2]) ? &argv[i][2] : argv[++i];
	      break;
	    case 'x':
	      dim_f |= 1;
	      opt->pict_pix_width = ('\0' != argv[i][2]) ? atoi(&argv[i][2]) : atoi(argv[++i]);
	      break;
	    case 'y':
	      dim_f |= 2;
	      opt->pict_pix_height = ('\0' != argv[i][2]) ? atoi(&argv[i][2]) : atoi(argv[++i]);
	      break;
	    case 'w':
	      dim_f |= 4;
	      opt->pict_real_width = ('\0' != argv[i][2]) ? atof(&argv[i][2]) : atof(argv[++i]);
	      break;
	    case 'h':
	      dim_f |= 8;
	      opt->pict_real_height = ('\0' != argv[i][2]) ? atof(&argv[i][2]) : atof(argv[++i]);
	      break;
	    case 'z':
	      opt->zoom = ('\0' != argv[i][2]) ? atoi(&argv[i][2]) : atoi(argv[++i]);
	      break;
	    case 'r':
	      opt->reflection_depth = ('\0' != argv[i][2]) ? atoi(&argv[i][2]) : atoi(argv[++i]);
	      break;
	    case 'q':
	      opt->reflection_depth = 0;
	      opt->shading = NoShading;
	      break;
	    case '?':
	      _options_print_usage(argv[0]);
	      return NULL;
	    default:
	      fprintf(stderr, "Ignoring unknown option \'-%c\'\n", argv[i][1]);
	    }
	}
    }

  /* Calculate image dimensions and resolution */
  switch (dim_f)
    {
    case 15: /* 1111 All dimensions defined */
      opt->rx = opt->pict_real_width / (real) opt->pict_pix_width;
      opt->ry = opt->pict_real_height / (real) opt->pict_pix_height;
      break;
    case 14: /* 1110, x not defined */
      opt->ry = opt->pict_real_height / (real) opt->pict_pix_height;
      opt->rx = opt->ry;
      opt->pict_pix_width = opt->pict_real_width / opt->rx;
      break;
    case 13: /* 1101 y not defined */
      opt->rx = opt->pict_real_width / (real) opt->pict_pix_width;
      opt->ry = opt->rx;
      opt->pict_pix_height = opt->pict_real_height / opt->ry;
      break;
    case 11: /* 1011 w not defined */
      opt->ry = opt->pict_real_height / (real) opt->pict_pix_height;
      opt->rx = opt->ry;
      opt->pict_real_width = (real) opt->pict_pix_width * opt->rx;
      break;
    case 7: /* 0111 h not defined */
      opt->rx = opt->pict_real_width / (real) opt->pict_pix_width;
      opt->ry = opt->rx;
      opt->pict_real_height = (real) opt->pict_pix_height * opt->ry;
      break;
    default:
      _options_print_usage(argv[0]);
      return NULL;
    }

  return opt;
}

int options_get_pict_pix_width(options opt)
{
   return (opt->pict_pix_width);
}

int options_get_pict_pix_height(options opt)
{
   return (opt->pict_pix_height);
}

real options_get_pict_real_width(options opt)
{
   return (opt->pict_real_width);
}

real options_get_pict_real_height(options opt)
{
   return (opt->pict_real_height);
}

real options_get_zoom(options opt)
{
   return (opt->zoom);
}

int options_get_reflection_depth(options opt)
{
   return (opt->reflection_depth);
}

real options_get_rx(options opt)
{
   return (opt->rx);
}

real options_get_ry(options opt)
{
   return (opt->ry);
}

char *options_get_in_file_name(options opt)
{
   return (opt->in_file_name);
}

char *options_get_out_file_name(options opt)
{
   return (opt->out_file_name);
}

char *options_get_comment(options opt)
{
   return (opt->comment);
}

shading_mode options_get_shading(options opt)
{
  return (opt->shading);
}
