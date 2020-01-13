#include "vector.h"
#include "surface.h"
#include "hitdata.h"
#include "ray.h"
#include "object_type.h"

#ifndef _PLANE_H_
#define _PLANE_H_

typedef struct plane_t* plane;
struct plane_t
{
  object_type type;
  vector normal;
  vector point;
  surface surf;
};

plane plane_create(vector norm, vector point, surface surf);

vector plane_get_normal(plane obj);
vector plane_get_point(plane obj);
surface plane_get_surface(plane obj);

real plane_hit_distance(plane o, ray r);
hitdata plane_hitdata(plane o, ray r);

#endif /* _PLANE_H_ */
