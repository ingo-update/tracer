#include <math.h>

#include "hitdata.h"

hitdata hitdata_create(vector norm, vector point, color col, real reflection, real diffuse, real angle)
{
  hitdata h;

  h.normal = norm;
  h.hit_point = point;
  h.col = col;
  h.reflection = reflection;
  h.diffuse = diffuse;
  h.angle = angle;

  return h;
}

hitdata hitdata_create_miss(color c)
{
  hitdata 	h;

  h.normal = vector_create(1,1,1);
  h.hit_point = vector_create(NO_HIT,0,0);
  h.col = c;
  h.reflection = 0;
  h.diffuse = 1;
  h.angle = M_PI/2;

  return h;
}

int hitdata_is_miss(hitdata h)
{
  return (NO_HIT == vector_get_x(h.hit_point));
}

vector hitdata_get_normal(hitdata h)
{
  return h.normal;
}

vector hitdata_get_hit_point(hitdata h)
{
  return h.hit_point;
}

color hitdata_get_color(hitdata h)
{
  return h.col;
}

real hitdata_get_reflection (hitdata h)
{
  return h.reflection;
}

real hitdata_get_diffuse (hitdata h)
{
  return h.diffuse;
}

real hitdata_get_angle(hitdata h)
{
   return h.angle;
}

// TODO: Check this...
hitdata hitdata_put_color(hitdata h, color col) {
  h.col = col;
  return h;
}
