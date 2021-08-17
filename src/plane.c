#include "plane.h"

/* Create triangle or parallelogram */
static plane _plane_create3(vector c0, vector a, vector b, surface surf, object_type t)
{
  plane new;

  if (NULL == surf)
    {
      fprintf(stderr, "_plane_create3(): Illegal surface modifier\n");
      exit(EXIT_FAILURE);
    }

  new = (plane) malloc(sizeof(struct plane_t));
  if (NULL == new)
    {
      fprintf(stderr,"_plane_create3(): Could not allocate plane\n");
      exit(EXIT_FAILURE);
    }

  new->type = t;
  new->corner0 = c0;
  new->a_leg = a;
  new->another_leg = b;
  new->normal = vector_norm(vector_xp(a, b)); // 29
  new->surf = surf;

  return new;
}

/* Create infinite plane */
plane plane_create(vector norm, vector point, surface surf)
{
 plane new;

  if (NULL == surf)
    {
      fprintf(stderr, "plane_create(): Illegal surface modifier\n");
      exit(EXIT_FAILURE);
    }

  new = (plane) malloc(sizeof(struct plane_t));
  if (NULL == new)
    {
      fprintf(stderr,"plane_create(): Could not allocate plane\n");
      exit(EXIT_FAILURE);
    }

  new->type = Plane;
  new->normal = vector_norm(norm);
  new->corner0 = point;
  new->surf = surf;

  return new;

}
plane triangle_create(vector c0, vector a, vector b, surface surf)
{
  return _plane_create3(c0, a, b, surf, Triangle);
}

plane pgram_create(vector c0, vector a, vector b, surface surf)
{
  return _plane_create3(c0, a, b, surf, Parallelogram);
}

hitdata plane_hitdata(plane o, ray r, tracing_mode m)
{
  object_type type;
  vector a, b, c0, q, rd, r0, pn, rn, ri;
  real 	d, u, v, vd, v0, t;
  real 	a2, b2, ab, qa, qb;

  surface surf;
  bitmap bmp;
  unsigned int x_in_map, y_in_map;
  color col;

  rd = ray_get_direction(r);
  r0 = ray_get_origin(r);

  c0 = plane_get_corner0(o);
  pn = plane_get_normal(o);

  vd = vector_dp(pn, rd);

  // Is ray parallel to plane?
  if (0 == vd) return HITDATA_MISS;

  d = vector_dp(c0, pn); // 30
  v0 = d - vector_dp(pn, r0);
  t = v0 / vd;

  // Is ray away from plane?
  if (0 > t) return HITDATA_MISS;

  ri = vector_sum(r0, vector_sp(rd, t));
  rn = (0 > vd) ? pn : vector_sp(pn, -1);

  // Outside triangle / parallelogram?
  type = plane_get_type(o);
  if (Plane != type)
    {
      a = plane_get_a_leg(o);
      b = plane_get_another_leg(o);

      q = vector_diff(ri, c0); // 31
      b2 = vector_dp(b, b);
      qa = vector_dp(q, a);
      ab = vector_dp(a, b);
      qb = vector_dp(q, b);
      a2 = vector_dp(a, a);

      u = (b2 * qa - ab * qb) / (a2 * b2 - ab * ab); // 32
      if (0 >= u || 1 <= u) return HITDATA_MISS;
      v = (qb - u * ab) / b2; // 33
      if (0 >= v || 1 <= v || ((Parallelogram != type) && (1 < (u+v)))) return HITDATA_MISS;
    }
  else u = v = 0; // To keep the compiler happy...

  // Only tracing for distance?
  if (Distance == m) return hitdata_distance(t);

  surf = plane_get_surface(o);

  if (Color == surface_get_mode(surf)) col = surface_get_color(surf);
  else // Texture map
    {
      bmp = surface_get_texture_map(surf);
      x_in_map = (bitmap_width(bmp) - 1) * u;
      y_in_map = (bitmap_height(bmp) - 1) * v;
      col = bitmap_get_pixel(bmp, x_in_map, y_in_map);
    }

  return hitdata_create(rn,
			vector_sum(r0,vector_sp(rd,t)),
			col,
			t,
			surface_get_reflection(surf),
			surface_get_diffuse(surf),
			fabs(vector_dp(rn, rd))
			);
}
