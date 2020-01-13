#include <math.h>

#include "hitdata.h"

hitdata hitdata_create(vector norm, vector point, color col, real distance, real reflection, real diffuse, real angle)
{
  hitdata h;

  h.normal = norm;
  h.hit_point = point;
  h.col = col;
  h.distance = distance;
  h.reflection = reflection;
  h.diffuse = diffuse;
  h.angle = angle;

  return h;
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

real hitdata_get_distance (hitdata h)
{
  return h.distance;
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
