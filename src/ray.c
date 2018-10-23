#include "ray.h"

ray ray_create(vector o, vector d)
{
  ray r;

  r.origin = o;
  r.direction = d;

  return r;
}

vector ray_get_origin(ray r)
{
  return r.origin;
}

vector ray_get_direction(ray r)
{
  return r.direction;
}
