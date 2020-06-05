#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "plane.h"

plane plane_create(vector norm, vector point, surface surf)
{
  plane new;

  if (NULL == surf) // TODO: Can this really happen?
    {
      fprintf(stderr, "plane_create(): Illegal surface modifier\n");
      exit(EXIT_FAILURE);
    }

  new = (plane) malloc(sizeof(struct plane_t));
  if (NULL == new) {
    fprintf(stderr,"plane_create(): Could not allocate plane\n");
    exit(EXIT_FAILURE);
  }

  new->type = Plane;
  new->normal = vector_norm(norm);
  new->point = point;
  new->surf = surf;

  return new;
}

surface plane_get_surface(plane obj)
{
  return obj->surf;
}

hitdata plane_hitdata(plane o, ray r, tracing_mode m)
{
  vector c0, rd, r0, pn, rn;
  real d, vd, v0, t;
  hitdata data;

  rd = ray_get_direction(r);
  r0 = ray_get_origin(r);

  c0 = o->point;
  pn = o->normal;

  vd = vector_dp(pn, rd); // 24

  if (0 == vd) // ray is parallel to plane
    {
      data.distance = NO_HIT;
      return data;
    }

  d = vector_dp(c0, pn); // 23
  v0 = d - vector_dp(pn, r0); // 25
  t = v0 / vd; // 26

  if (0 > t) // ray is away from plane
    {
      data.distance = NO_HIT;
      return data;
    }

  data.distance = t;
  if (Distance == m) return data;

  // Hit point normal.
  rn = (0 < vd) ? vector_sp(pn, -1) : pn;

  // TODO: Do this with hitpoint_create instead?
  data.normal = rn;
  data.hit_point = vector_sum(r0,vector_sp(rd, t));
  data.col = surface_get_color(o->surf);
  data.reflection = surface_get_reflection(o->surf);
  data.diffuse = surface_get_diffuse(o->surf);
  data.angle = fabs(vector_dp(rn, rd));

  return data;
}
