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

#define sphere_get_radius(o) o->radius
#define sphere_get_location(o) o->location
#define sphere_get_surface(o) o->surf
#define sphere_get_pole(o) o->pole
#define sphere_get_equator(o) o->equator

#define sph_opt_get_pole(o) o.pole
#define sph_opt_get_equator(o) o.equator

typedef struct sphere_t* sphere;
struct sphere_t
{
  object_type type;
  vector pole;
  vector equator;
  vector location;
  real radius;
  surface surf;
};

sphere sphere_create(vector pole, vector equator, vector loc, real rad, surface surf);

hitdata sphere_hitdata(sphere o, ray r, tracing_mode m);

/*  Sphere opt, help for the parser */
struct sph_opt
{
  vector pole;
  vector equator;
};

struct sph_opt sph_opt_create(vector pole, vector equator);

#endif /* _SPHERE_H_ */
