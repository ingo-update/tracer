#include "real.h"

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
  shading_mode shading;
};

options options_get_options(int argc, char **argv);

int options_get_pict_pix_width(options opt);
int options_get_pict_pix_height(options opt);
real options_get_pict_real_width(options opt);
real options_get_pict_real_height(options opt);
real options_get_zoom(options opt);
int options_get_reflection_depth(options opt);
real options_get_rx(options opt);
real options_get_ry(options opt);
char *options_get_in_file_name(options opt);
char *options_get_out_file_name(options opt);
char *options_get_comment(options opt);
shading_mode options_get_shading(options opt);

#endif /* _OPTIONS_H_ */
