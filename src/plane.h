#include "vector.h"
#include "surface.h"
#include "ray.h"
#include "hitdata.h"

#ifndef _PLANE_H_
#define _PLANE_H_

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

vector plane_get_corner0(plane obj);
vector plane_get_a_leg(plane obj);
vector plane_get_another_leg(plane obj);
vector plane_get_normal(plane obj);
surface plane_get_surface(plane obj);

hitdata plane_hitdata(plane o, ray r, tracing_mode m);

#endif /* _PLANE_H_ */
