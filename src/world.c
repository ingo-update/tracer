#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "defaults.h"
#include "world.h"

static struct list *_cons(void *car, struct list *cdr)
{
  struct list *c;
  c = (struct list *) malloc(sizeof(struct list));

  if (NULL == c)
    {
      fprintf(stderr,"_cons(): Could not allocate mamory\n");
      return NULL;
    }

  c->car = car;
  c->cdr = cdr;

  return c;
}

world world_create()
{
  world w;
  w = (world) malloc(sizeof(struct world_t));

  if (NULL == w) {
    fprintf(stderr,"world_create(): Could not allocate memory\n");
    return NULL;
  }

  w->bg_color = DEF_BG_COLOR;
  w->objects = NULL;
  w->lights = NULL;

  return w;
}

void world_add_object(world w, object o)
{
  w->objects = _cons(o, (w->objects));
}

void world_add_light(world w, light l)
{
  w->lights = _cons(l, (w->lights));
}

color world_get_background(world w)
{
   return w->bg_color;
}

void world_put_background(world w, color col)
{
  w->bg_color = col;
}

static color _illumination(world w, hitdata hit)
{
  struct list *objs, *lights;
  light c_light;
  color c_light_color, acc_light_color;
  vector point, c_light_dir;
  ray ray_to_c_light;
  real c_light_dist, c_light_angle, c_obj_dist;

  point = vector_sum(hitdata_get_hit_point(hit), vector_sp(hitdata_get_normal(hit), OFFSET));

  /* Accumulate light from all light sources */
  acc_light_color = color_create_rgb(0,0,0);
  for (lights = w->lights ; lights ; lights = lights->cdr)
    {
      c_light = lights->car;
      c_light_dir = light_direction(c_light, point);
      c_light_dist = vector_length(c_light_dir);
      c_light_dir = vector_norm(c_light_dir);

      ray_to_c_light = ray_create(point, c_light_dir);

      /* Find any object blocking the light */
      c_obj_dist = NO_HIT;
      for (objs = w->objects ; (objs && c_obj_dist > c_light_dist) ; objs = objs->cdr)
	{
	  c_obj_dist = object_hit_distance(objs->car, ray_to_c_light);
	}

      /* Add this light if no blocking object was found */
      if (!objs)
	{
	  c_light_angle = fabs(vector_dp(hitdata_get_normal(hit),c_light_dir));
	  c_light_color = color_scale(light_get_color(c_light), c_light_angle);
	  acc_light_color = color_add(acc_light_color, c_light_color);
	}
    }

  return acc_light_color;
}

color world_look(world w, ray r, unsigned int depth, shading_mode s)
{
  struct list *objs;
  object closest_obj;
  real dist, closest_dist;
  vector norm, dir, ref_dir, ref_loc;
  color diffuse_color;
  hitdata hit;

  closest_obj = NULL;
  closest_dist = NO_HIT;

  /* Find closest object in ray */
  for (objs = w->objects ; objs ; objs = objs->cdr)
    {
      dist = object_hit_distance(objs->car, r);
      if (NO_HIT != dist && dist < closest_dist)
	{
	  closest_dist = dist;
	  closest_obj = objs->car;
	}
    }

  /* Return background if no object was hit by the ray */
  if (!closest_obj) return world_get_background(w);

  /* Calculate the hit */
  hit = object_hitdata(closest_obj, r);

  /* Quick mode? */
  if (Shading != s) return color_scale(hitdata_get_color(hit), hitdata_get_angle(hit));

  /* Diffuse */
  diffuse_color = color_multiply(color_scale(_illumination(w, hit),hitdata_get_diffuse(hit)), hitdata_get_color(hit));

  /* Done with reflection? */
  if (0 == depth || 0 == hitdata_get_reflection(hit)) return diffuse_color;

  /* Reflection (recursive) */
  dir = ray_get_direction(r);
  norm = hitdata_get_normal(hit);
  ref_dir = vector_sum(dir, vector_sp(vector_sp(norm, vector_dp(norm, dir)), -2));
  ref_loc = vector_sum(hitdata_get_hit_point(hit), vector_sp(norm, OFFSET));

  return color_add(diffuse_color, color_scale(world_look(w, ray_create(ref_loc, ref_dir), depth-1, s), hitdata_get_reflection(hit)));
}
