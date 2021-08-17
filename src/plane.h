#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector.h"
#include "surface.h"
#include "ray.h"
#include "hitdata.h"

#ifndef _PLANE_H_
#define _PLANE_H_

#define plane_get_type(o) o->type
#define plane_get_corner0(o) o->corner0
#define plane_get_a_leg(o) o->a_leg
#define plane_get_another_leg(o) o->another_leg
#define plane_get_normal(o) o->normal;
#define plane_get_surface(o) o->surf;

typedef struct plane_t* plane;
struct plane_t
{
  object_type type;
  vector corner0;
  vector a_leg;
  vector another_leg;
  vector normal;
  surface surf;
};

plane plane_create(vector c0, vector normal, surface surf);
plane triangle_create(vector c0, vector a, vector b, surface surf);
plane pgram_create(vector c0, vector a, vector b, surface surf);

hitdata plane_hitdata(plane o, ray r, tracing_mode m);

#endif /* _PLANE_H_ */
