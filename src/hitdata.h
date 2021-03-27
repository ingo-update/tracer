#include <math.h>
#include <limits.h>

#include "types.h"
#include "vector.h"
#include "color.h"

#ifndef _HITDATA_H_
#define _HITDATA_H_

#define NO_HIT REAL_MAX
#define HITDATA_MISS hitdata_distance(NO_HIT)

typedef struct hitdata_t hitdata;

struct	hitdata_t
{
  vector normal, hit_point;
  color col;
  real distance, reflection, diffuse, angle;
};

hitdata hitdata_distance(real distance);
hitdata hitdata_create(vector norm, vector point, color col, real distance, real reflection, real diffuse, real angle);

vector hitdata_get_normal(hitdata data);
vector  hitdata_get_hit_point(hitdata data);
color hitdata_get_color(hitdata data);
real hitdata_get_distance(hitdata data);
real hitdata_get_reflection (hitdata data);
real hitdata_get_diffuse (hitdata data);
real hitdata_get_angle(hitdata data);

hitdata hitdata_put_color(hitdata data, color col);

#endif /* _HITDATA_H_ */
