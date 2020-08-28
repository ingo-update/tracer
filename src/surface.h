#include "color.h"
#include "bitmap.h"
#include "types.h"
#include "defaults.h"

#ifndef _SURFACE_H_
#define _SURFACE_H_

typedef enum {Color, TextureMap} surface_mode;

typedef struct surface_t* surface;
struct surface_t {
  surface_mode mode;
  color col;
  char *texture_name;
  bitmap texture;
  real reflection;
  real diffuse;
};

struct pigment {
  surface_mode type;
  color col;
  char *texture_name;
};

struct finish {
  real reflection;
  real diffuse;
};

surface surface_create(struct pigment pig, struct finish fin);

surface_mode surface_get_mode(surface s);
color surface_get_color(surface s);
bitmap surface_get_texture_map(surface s);
real surface_get_reflection(surface s);
real surface_get_diffuse(surface s);

/* Parser help functions */

struct pigment _pigment_create_color(color c);
struct pigment _pigment_create_texturemap(char *name);
struct finish _finish_create(real ref, real dif);

#endif /* _SURFACE_H_ */
