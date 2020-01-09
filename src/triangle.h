#include "vector.h"
#include "surface.h"
#include "ray.h"
#include "hitdata.h"
#include "object_type.h"

#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

typedef struct triangle_t* triangle;
struct triangle_t {
  object_type type;
  vector corner0;
  vector a_leg;
  vector another_leg;
  surface surf;
  int pgram; /* 1 if parallelogram, 0 if triangle */
};

triangle triangle_create(vector c0, vector a, vector b, surface surf);
triangle pgram_create(vector c0, vector a, vector b, surface surf);

vector triangle_get_corner0(triangle obj);
vector triangle_get_a_leg(triangle obj);
vector triangle_get_another_leg(triangle obj);
surface triangle_get_surface(triangle obj);

real triangle_hit_distance(triangle o, ray r);
hitdata triangle_hitdata(triangle o, ray r);

#endif /* _TRIANGLE_H_ */
