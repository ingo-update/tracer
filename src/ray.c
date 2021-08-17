#include "ray.h"

ray ray_create(vector o, vector d)
{
  ray r;

  r.origin = o;
  r.direction = d;

  return r;
}
