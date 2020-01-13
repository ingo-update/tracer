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

  new->type = Sphere;
  new->pole = pole;
  new->equator = equator;
  new->location = loc;
  new->radius = rad;
  new->surf = surf;

  return new;
}

struct sph_opt sph_opt_create(vector pole, vector equator)
{
  struct sph_opt o;

  o.pole = pole;
  o.equator = equator;

  return o;
}

real sphere_hit_distance(sphere o, ray r)
{
  hitdata h;
  h = sphere_hitdata(o, r);
  return h.distance;
}

hitdata sphere_hitdata(sphere o, ray r)
{
  int inside;
  vector oc, location, origin;
  real l_2oc, t_ca, t_2hc, radius, t;

  vector ri, rn;
  color col;

  unsigned int x_in_map, y_in_map;
  real fi, v, u, theta;
  vector sp, se;
  bitmap bmp;

  hitdata data;

  radius = o->radius;
  location = o->location;
  origin = ray_get_origin(r);

  oc = vector_diff(location, origin); // 16
  l_2oc = vector_dp(oc, oc); // 17

  inside = (l_2oc < radius * radius) ? 1 : 0;

  t_ca = vector_dp(oc, ray_get_direction(r)); // 18
  if (!inside && 0 > t_ca)
    {
      // looking away from sphere
      data.distance = NO_HIT;
      return data;
    }

  t_2hc = (radius * radius) - l_2oc + (t_ca * t_ca); // 19
  if (0 > t_2hc)
    {
      // looking past sphere
      data.distance = NO_HIT;
      return data;
    }

  t = inside ? t_ca + sqrt(t_2hc) : t_ca - sqrt(t_2hc); // 2
  data.distance = t;

  ri = vector_sum(origin, vector_sp(ray_get_direction(r), t));
  rn = vector_sp(vector_diff(ri, location), (inside ? -1 : 1) / radius);

  if (Color == surface_get_mode(o->surf)) col = surface_get_color(o->surf);
  else // Texture map
    {
      sp = o->pole;
      se = o->equator;
      bmp = surface_get_texture_map(o->surf);

      /* Find latitude, 0.0 = N, 1.0 = S */
      fi = acos(-vector_dp(rn, sp)); // 34
      v = fi / M_PI; // 35

      /* Find longitude, 0.0 = E, 1.0 = W */
      if (0.0 == v || 1.0 == v)  u = 0.0; // On a pole TODO: Can this happen?
      else
	{
	  theta = (acos(vector_dp(se, rn) / sin(fi)) / (2.0 * M_PI));
	  u = (0.0 < vector_dp(vector_xp(se, sp), rn)) ? theta : 1.0 - theta;

	  // - 1 because of zeroindexed bitmaps
	  x_in_map = (bitmap_width(bmp) - 1) * u;
	  y_in_map = (bitmap_height(bmp) - 1) * v;
	  col = bitmap_get_pixel(bmp, x_in_map, y_in_map);
	}
    }

  data.normal = rn;
  data.hit_point = vector_sum(origin, vector_sp(ray_get_direction(r), t));
  data.col = col;
  data.reflection = surface_get_reflection(o->surf);
  data.diffuse = surface_get_diffuse(o->surf);
  data.angle = fabs(vector_dp(rn, ray_get_direction(r)));

  return data;
}

vector sph_opt_get_pole(struct sph_opt o)
{
  return o.pole;
}

vector sph_opt_get_equator(struct sph_opt o)
{
  return o.equator;
}
