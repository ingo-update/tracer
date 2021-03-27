#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector.h"
#include "surface.h"
#include "ray.h"
#include "hitdata.h"

#ifndef _PLANE_H_
#define _PLANE_H_

#define plane_get_corner0(p) ((plane) p)->corner0
#define plane_get_a_leg(p) ((plane) p)->a_leg
#define plane_get_another_leg(p) ((plane) p)->another_leg
#define plane_get_normal(p) ((plane) p)->normal;
#define plane_get_surface(p) ((plane) p)->surf;

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
