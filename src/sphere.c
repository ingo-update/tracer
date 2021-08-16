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

hitdata sphere_hitdata(sphere o, ray r, tracing_mode m)
{
  int inside;
  vector oc, location, origin;
  real l_2oc, t_ca, t_2hc, radius, t;

  surface surf;

  vector ri, rn;
  color col;

  unsigned int x_in_map, y_in_map;
  real fi, v, u, theta;
  vector sp, se;
  bitmap bmp;

  radius = sphere_get_radius(o);
  location = sphere_get_location(o);
  origin = ray_get_origin(r);

  oc = vector_diff(location, origin); // 16
  l_2oc = vector_dp(oc, oc); // 17

  // Are we inside the sphere?
  inside = (l_2oc < radius * radius);

  t_ca = vector_dp(oc, ray_get_direction(r)); // 18

  // Looking away from sphere?
  if (!inside && 0 > t_ca) return HITDATA_MISS;

  t_2hc = (radius * radius) - l_2oc + (t_ca * t_ca); // 19

  // Looking past sphere?
  if (0 > t_2hc) return HITDATA_MISS;

  t = inside ? t_ca + sqrt(t_2hc) : t_ca - sqrt(t_2hc); // 2

  // Only tracing for distance?
  if (Distance == m) return hitdata_distance(t);

  ri = vector_sum(origin, vector_sp(ray_get_direction(r), t));
  rn = vector_sp(vector_diff(ri, location), (inside ? -1 : 1) / radius);

  surf = sphere_get_surface(o);

  if (Color == surface_get_mode(surf)) col = surface_get_color(surf);
  else // Texture map
    {
      sp = sphere_get_pole(o);
      se = sphere_get_equator(o);
      bmp = surface_get_texture_map(surf);

      /* Find latitude, 0.0 = N, 1.0 = S */
      fi = acos(-vector_dp(rn, sp)); // 34
      v = fi / M_PI; // 35

      /* Find longitude, 0.0 = E, 1.0 = W */
      if (0.0 == v || 1.0 == v)  u = 0.0; /* On a pole, longitude is not defined so take any */
      else
	{
	  theta = (acos(vector_dp(se, rn) / sin(fi)) / (2.0 * M_PI));
	  u = (0.0 < vector_dp(vector_xp(se, sp), rn)) ? theta : 1.0 - theta;
	}

      // - 1 because of zeroindexed bitmaps
      x_in_map = (bitmap_width(bmp) - 1) * u;
      y_in_map = (bitmap_height(bmp) - 1) * v;
      col = bitmap_get_pixel(bmp, x_in_map, y_in_map);
    }

  return hitdata_create(rn,
			vector_sum(origin, vector_sp(ray_get_direction(r), t)),
			col,
			t,
			surface_get_reflection(surf),
			surface_get_diffuse(surf),
			fabs(vector_dp(rn, ray_get_direction(r)))
			);
}
