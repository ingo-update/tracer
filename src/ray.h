#include "vector.h"

#ifndef _RAY_H_
#define _RAY_H_

struct ray_t
{
  vector origin;
  vector direction;
};

typedef struct ray_t ray;

ray ray_create(vector o, vector d);

vector ray_get_origin(ray r);
vector ray_get_direction(ray r);

#endif /* _RAY_H_ */
