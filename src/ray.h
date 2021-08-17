#include "vector.h"

#ifndef _RAY_H_
#define _RAY_H_

#define ray_get_origin(r) r.origin
#define ray_get_direction(r) r.direction

typedef struct ray_t ray;
struct ray_t
{
  vector origin;
  vector direction;
};

ray ray_create(vector o, vector d);

#endif /* _RAY_H_ */
