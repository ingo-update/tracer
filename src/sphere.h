#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "vector.h"
#include "surface.h"
#include "ray.h"
#include "hitdata.h"

#ifndef _SPHERE_H_
#define _SPHERE_H_

typedef struct sphere_t* sphere;

struct sphere_t {
  object_type type;
  vector pole;
  vector equator;
  vector location;
  real radius;
  surface surf;
};

/*  Sphere opt, help for the parser */

struct sph_opt {
  vector pole;
  vector equator;
};

struct sph_opt sph_opt_create(vector pole, vector equator);

sphere sphere_create(vector pole, vector equator, vector loc, real rad, surface surf);

vector sphere_get_pole(sphere obj);
vector sphere_get_equator(sphere obj);
vector sphere_get_location(sphere obj);
real sphere_get_radius(sphere obj);
surface sphere_get_surface(sphere o);

hitdata sphere_hitdata(sphere o, ray r, tracing_mode m);

vector sph_opt_get_pole(struct sph_opt o);
vector sph_opt_get_equator(struct sph_opt o);

#endif /* _SPHERE_H_ */
