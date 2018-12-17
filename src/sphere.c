#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sphere.h"

sphere sphere_create(vector pole, vector equator, vector loc, real rad, surface surf)
{
  sphere new;

  if (NULL == surf)
    {
      fprintf(stderr, "sphere_create(): Illegal surface modifier\n");
      exit(EXIT_FAILURE);
    }

  new = (sphere) malloc(sizeof(struct sphere_t));

  if (NULL == new)
    {
      fprintf(stderr, "sphere_create(): Could not allocate sphere\n");
      exit(EXIT_FAILURE);
    }

  new->pole = pole;
  new->equator = equator;
  new->location = loc;
  new->radius = rad;
  new->surf = surf;

  return(new);
}

struct sph_opt sph_opt_create(vector pole, vector equator)
{
  struct sph_opt o;

  o.pole = pole;
  o.equator = equator;

  return(o);
}

real sphere_hit_distance(sphere o, ray r)
{
  int side;
  vector oc, location, origin;
  real l_2oc, t_ca, t_2hc, t, radius;

  side = 0; // This means outside
  radius = o->radius;
  location = o->location;
  origin = ray_get_origin(r);

  oc = vector_diff(location, origin); // 16
  l_2oc = vector_dp(oc, oc); // 17

  if (l_2oc < radius * radius)
    {
      side = 1; // This means inside
    }

  t_ca = vector_dp(oc, ray_get_direction(r)); // 18

  if (0 > t_ca && 0 == side) // Looking away from sphere
    {
      return NO_HIT;
    }

  t_2hc = (radius * radius) - l_2oc + (t_ca * t_ca); // 19

  if (0 > t_2hc) // looking past sphere
    {
      return NO_HIT;
    }

  // t is the distance
  if (0 == side) // 20
    {
      t = t_ca - sqrt(t_2hc);
    }
  else
    {
      t = t_ca + sqrt(t_2hc);
    }

  return t;
}

hitdata sphere_hitdata(sphere o, ray r)
{
  int side;
  vector oc, location, origin;
  real l_2oc, t_ca, t_2hc, t, radius;

  vector ri, rn;
  color col;

  unsigned int x_in_map, y_in_map;
  real fi, v, u, theta;
  vector sp, se;
  bitmap bmp;

  side = 0; // This means outside
  radius = o->radius;
  location = o->location;
  origin = ray_get_origin(r);

  oc = vector_diff(location, origin); // 16
  l_2oc = vector_dp(oc, oc); // 17

  if (l_2oc < radius * radius)
    {
      side = 1; // This means inside
    }

  t_ca = vector_dp(oc, ray_get_direction(r)); // 18
  t_2hc = (radius * radius) - l_2oc + (t_ca * t_ca); // 19

  if (0 == side) // 20
    {
      t = t_ca - sqrt(t_2hc);
    }
  else
    {
      t = t_ca + sqrt(t_2hc);
    }

  ri = vector_sum(origin, vector_sp(ray_get_direction(r), t));

  if (0 == side)
    {
      rn = vector_sp(vector_diff(ri, location), (1 / radius));
    }
  else
    {
      rn = vector_sp(vector_diff(ri, location), (-1 / radius));
    }

  if (Color == surface_get_mode(o->surf))
    {
      col = surface_get_color(o->surf);
    }
  else
    {
      // sphere with texture map
      sp = o->pole;
      se = o->equator;
      bmp = surface_get_texture_map(o->surf);

      fi = acos(-vector_dp(rn, sp)); // 34
      v = fi / M_PI;		// 35

      if (0.0 == v || 1.0 == v) // On a pole TODO: Can this happen?
	{
	  u = 0.0;
	}
      else
	{
	  theta = (acos(vector_dp(se, rn) / sin(fi)) / (2.0 * M_PI));
	  if (0.0 < vector_dp(vector_xp(se, sp), rn))
	    {
	      u = theta;
	    }
	  else
	    {
	      u = 1.0 - theta;
	    }

	  // - 1 because of zeroindexed bitmaps
	  x_in_map = (bitmap_width(bmp) - 1) * u;
	  y_in_map = (bitmap_height(bmp) - 1) * v;
	  col = bitmap_get_pixel(bmp, x_in_map, y_in_map);
      }
    }

  return hitdata_create(rn,
			vector_sum(origin, vector_sp(ray_get_direction(r), t)),
			col,
			surface_get_reflection(o->surf),
			surface_get_diffuse(o->surf),
			fabs(vector_dp(rn, ray_get_direction(r)))
			);
}

vector sph_opt_get_pole(struct sph_opt o)
{
  return (o.pole);
}

vector sph_opt_get_equator(struct sph_opt o)
{
  return(o.equator);
}
