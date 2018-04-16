#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "world.h"
#include "real.h"
#include "ray.h"
#include "hitdata.h"
#include "surface.h"
#include "pgram.h"

pgram pgram_create(vector c0, vector a, vector b, surface surf)
{
  pgram new;
  
  if (NULL == surf)
    {
      fprintf(stderr, "pgram_create(): Illegal surface modifier\n");
      exit(EXIT_FAILURE);
    }

  new = (pgram) malloc(sizeof(struct pgram_t));
  if (NULL == new)
    {
      fprintf(stderr,"prgam_create(): Could not allocate pgram\n");
      exit(EXIT_FAILURE);
    }
  
  new->corner0 = c0;
  new->a_leg = a;
  new->another_leg = b;
  new->surf = surf;
  
  return new;
}

vector pgram_get_corner0(pgram obj)
{
   return obj->corner0;
}

vector pgram_get_a_leg(pgram obj)
{
   return obj->a_leg;
}

vector pgram_get_another_leg(pgram obj)
{
   return obj->another_leg;
}

surface pgram_get_surface(pgram obj)
{
  return obj->surf;
}

real pgram_hit_distance(pgram o, ray r) {
  vector a, b, c0, q, rd, r0, pn, ri;
  real d, u, v, vd, v0, t;
  real a2, b2, ab, qa, qb;

  rd = ray_get_direction(r);
  r0 = ray_get_origin(r);
  
  a = pgram_get_a_leg(o);
  b = pgram_get_another_leg(o);
  c0 = pgram_get_corner0(o);
  
  // Pgram plane normal
  pn = vector_norm(vector_xp(a, b)); // 29
  
  d = vector_dp(c0, pn); // 30
  vd = vector_dp(pn, rd);
  v0 = d - vector_dp(pn, r0);
  
  if (0 == vd) // ray is parallell
    {
      return NO_HIT;
    }
  
  t = v0 / vd;
  if (0 > t) // ray is away from plane
    {
      return NO_HIT;
    }
  
  // The ray hits the pgram plane
  
  ri = vector_sum(r0, vector_sp(rd, t));

  q = vector_diff(ri, c0); // 31
  b2 = vector_dp(b, b);
  qa = vector_dp(q, a);
  ab = vector_dp(a, b);
  qb = vector_dp(q, b);
  a2 = vector_dp(a, a);

  u = (b2 * qa - ab * qb) / (a2 * b2 - ab * ab); // 32
  if (0 >= u || 1 <= u)
    {
      return NO_HIT;
    }
  
  v = (qb - u * ab) / b2; // 33
  if (0 >= v || 1 <= v)
    {
      return NO_HIT;
    }
  
  return t;
}

hitdata pgram_hitdata(pgram o, ray r) {
  vector a, b, c0, q, rd, r0, pn, rn, ri;
  real 	d, u, v, vd, v0, t;
  real 	a2, b2, ab, qa, qb;

  hitdata data;
  bitmap bmp;		
  unsigned int x_in_map, y_in_map;
  color col;
  
  rd = ray_get_direction(r);
  r0 = ray_get_origin(r);

  a = pgram_get_a_leg(o);
  b = pgram_get_another_leg(o);
  c0 = pgram_get_corner0(o);

  pn = vector_norm(vector_xp(a, b)); // 29

  d = vector_dp(c0, pn); // 30
  vd = vector_dp(pn, rd);
  v0 = d - vector_dp(pn, r0);

  t = v0 / vd;
  ri = vector_sum(r0, vector_sp(rd, t));

  if (0 > vd)
    {
      rn = pn;
    }
  else
    {
      rn = vector_sp(pn, -1);
    }
  
  if (Color == surface_get_mode(o->surf))
    {
      col = surface_get_color(o->surf);
    }
  else
    {
      q = vector_diff(ri, c0); // 31
      b2 = vector_dp(b, b);
      qa = vector_dp(q, a);
      ab = vector_dp(a, b);
      qb = vector_dp(q, b);
      a2 = vector_dp(a, a);

      bmp = surface_get_texture_map(o->surf);
      
      u = (b2 * qa - ab * qb) / (a2 * b2 - ab * ab); // 32
      v = (qb - u * ab) / b2; // 33

      x_in_map = (bitmap_width(bmp) - 1) * u; 
      y_in_map = (bitmap_height(bmp) - 1) * v;
      
      col = bitmap_get_pixel(bmp, x_in_map, y_in_map);
    }
  
  data = hitdata_create(rn,
			vector_sum(r0,vector_sp(rd,t)),
			col,
			surface_get_reflection(o->surf),
			surface_get_diffuse(o->surf),
			fabs(vector_dp(rn, rd)));
  return data;
}
