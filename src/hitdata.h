#include <limits.h>

#include "real.h"
#include "vector.h"
#include "color.h"

#ifndef _HITDATA_H_
#define _HITDATA_H_

#define NO_HIT REAL_MAX

typedef struct hitdata_t hitdata;

struct	hitdata_t
{
  vector normal, hit_point;
  color col;
  real reflection, diffuse, angle;
};

hitdata hitdata_create(vector norm, vector point, color col, real reflection, real diffuse, real angle);
hitdata hitdata_create_miss();

int hitdata_is_miss(hitdata data);

vector hitdata_get_normal(hitdata data);
vector  hitdata_get_hit_point(hitdata data);
color hitdata_get_color(hitdata data);
real hitdata_get_reflection (hitdata data);
real hitdata_get_diffuse (hitdata data);
real hitdata_get_angle(hitdata data);

hitdata hitdata_put_color(hitdata data, color col);

#endif /* _HITDATA_H_ */
