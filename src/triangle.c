#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "triangle.h"

static triangle _tri_create(vector c0, vector a, vector b, surface surf, int pgram)
{
  triangle new;

  if (NULL == surf)
    {
      fprintf(stderr, "_tri_create(): Illegal surface modifier\n");
      exit(EXIT_FAILURE);
    }

  new = (triangle) malloc(sizeof(struct triangle_t));
  if (NULL == new)
    {
      fprintf(stderr,"_tri_create(): Could not allocate triangle\n");
      exit(EXIT_FAILURE);
    }

  new->type = Triangle;
  new->corner0 = c0;
  new->a_leg = a;
  new->another_leg = b;
  new->surf = surf;
  new->pgram = pgram;

  return new;
}

triangle triangle_create(vector c0, vector a, vector b, surface surf)
{
  return _tri_create(c0, a, b, surf, 0);
}

triangle pgram_create(vector c0, vector a, vector b, surface surf)
{
  return _tri_create(c0, a, b, surf, 1);
}

vector triangle_get_corner0(triangle obj)
{
   return obj->corner0;
}

vector triangle_get_a_leg(triangle obj)
{
   return obj->a_leg;
}

vector triangle_get_another_leg(triangle obj)
{
   return obj->another_leg;
}

surface triangle_get_surface(triangle obj)
{
  return obj->surf;
}

hitdata triangle_hitdata(triangle o, ray r, tracing_mode m)
{
  vector a, b, c0, q, rd, r0, pn, rn, ri;
  real 	d, u, v, vd, v0, t;
  real 	a2, b2, ab, qa, qb;

  hitdata data;
  bitmap bmp;
  unsigned int x_in_map, y_in_map;
  color col;

  data.distance = NO_HIT;

  rd = ray_get_direction(r);
  r0 = ray_get_origin(r);

  a = triangle_get_a_leg(o);
  b = triangle_get_another_leg(o);
  c0 = triangle_get_corner0(o);

  pn = vector_norm(vector_xp(a, b)); // 29

  d = vector_dp(c0, pn); // 30
  vd = vector_dp(pn, rd);
  v0 = d - vector_dp(pn, r0);

  // Is ray paralell to triangle plane?
  if (0 == vd) return data;

  t = v0 / vd;

  // Is ray away from triangle plane?
  if (0 > t) return data;

  ri = vector_sum(r0, vector_sp(rd, t));
  rn = (0 > vd) ? pn : vector_sp(pn, -1);

  q = vector_diff(ri, c0); // 31
  b2 = vector_dp(b, b);
  qa = vector_dp(q, a);
  ab = vector_dp(a, b);
  qb = vector_dp(q, b);
  a2 = vector_dp(a, a);

  u = (b2 * qa - ab * qb) / (a2 * b2 - ab * ab); // 32
  if (0 >= u || 1 <= u) return data;
  v = (qb - u * ab) / b2; // 33
  if (0 >= v || 1 <= v || (!(o->pgram) && (1 < (u+v)))) return data;

  data.distance = t;
  if (Distance == m) return data;

  if (Color == surface_get_mode(o->surf)) col = surface_get_color(o->surf);
  else
    {
      bmp = surface_get_texture_map(o->surf);
      x_in_map = (bitmap_width(bmp) - 1) * u;
      y_in_map = (bitmap_height(bmp) - 1) * v;
      col = bitmap_get_pixel(bmp, x_in_map, y_in_map);
    }

  data = hitdata_create(rn,
			vector_sum(r0,vector_sp(rd,t)),
			col,
			t,
			surface_get_reflection(o->surf),
			surface_get_diffuse(o->surf),
			fabs(vector_dp(rn, rd)));
  return data;
}
