#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defaults.h"
#include "bitmap.h"
#include "types.h"

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

typedef enum {Shading, NoShading} shading_mode;

typedef struct options_t* options;
struct options_t
{
  int pict_pix_width, pict_pix_height;
  real pict_real_width, pict_real_height;
  real zoom, rx, ry;
  int reflection_depth;
  char *in_file_name;
  char *out_file_name;
  char *comment;
  ppmtype ppm_mode;
  shading_mode shading;
};

#define options_get_pict_pix_width(opt)   (opt->pict_pix_width)
#define options_get_pict_pix_height(opt)  (opt->pict_pix_height)
#define options_get_pict_real_width(opt)  (opt->pict_real_width)
#define options_get_pict_real_height(opt) (opt->pict_real_height)
#define options_get_zoom(opt)             (opt->zoom)
#define options_get_reflection_depth(opt) (opt->reflection_depth)
#define options_get_rx(opt)               (opt->rx)
#define options_get_ry(opt)               (opt->ry)
#define options_get_in_file_name(opt)     (opt->in_file_name)
#define options_get_out_file_name(opt)    (opt->out_file_name)
#define options_get_comment(opt)          (opt->comment)
#define options_get_shading(opt)          (opt->shading)
#define options_get_ppm_mode(opt)         (opt->ppm_mode)

options options_get_options(int argc, char **argv);

#endif /* _OPTIONS_H_ */
