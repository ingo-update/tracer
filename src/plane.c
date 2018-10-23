#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "plane.h"

plane plane_create(vector norm, vector point, surface surf)
{
  plane new;

  if (NULL == surf)
    {
      fprintf(stderr, "plane_create(): Illegal surface modifier\n");
      exit(EXIT_FAILURE);
    }

  new = (plane) malloc(sizeof(struct plane_t));
  if (NULL == new) {
    fprintf(stderr,"plane_create(): Could not allocate plane\n");
    exit(EXIT_FAILURE);
  }

  new->normal = vector_norm(norm);
  new->point = point;
  new->surf = surf;

  return new;
}

real plane_hit_distance(plane o, ray r)
{
  vector c0, rd, r0, pn;
  real d, vd, v0, t;

  rd = ray_get_direction(r);
  r0 = ray_get_origin(r);

  c0 = o->point;
  pn = o->normal;

  d = vector_dp(c0, pn); // 23
  vd = vector_dp(pn, rd); // 24

  if (0 == vd) // ray is parallel
    {
      return NO_HIT;
    }

  v0 = d - vector_dp(pn, r0); // 25
  t = v0 / vd; // 26

  if (0 > t) // ray is away from plane
    {
      return NO_HIT;
    }

  return t;
}

hitdata plane_hitdata(plane o, ray r)
{
  vector c0, rd, r0, pn, rn;
  real d, vd, v0, t;
  hitdata data;

  rd = ray_get_direction(r);
  r0 = ray_get_origin(r);

  c0 = o->point;
  pn = o->normal;

  d = vector_dp(c0, pn); // 23
  vd = vector_dp(pn, rd); // 24

  v0 = d - vector_dp(pn, r0); // 25
  t = v0 / vd; // 26

  if (0 < vd)
    {
      rn = vector_sp(pn,-1);
    }
  else
    {
      rn = pn;
    }

  data = hitdata_create(rn,
			vector_sum(r0,vector_sp(rd, t)),
			surface_get_color(o->surf),
			surface_get_reflection(o->surf),
			surface_get_diffuse(o->surf),
			fabs(vector_dp(rn, rd)));
  return data;
}
