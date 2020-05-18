#include <stdio.h>
#include <stdlib.h>

#include "surface.h"

surface surface_create(struct pigment pig, struct finish fin)
{
  surface s;

  if (1 < fin.reflection || 0 > fin.reflection)
    {
      fprintf(stderr, "surface_create(): Illegal reflection value %f, should be between 0 and 1.\n", fin.reflection);
      return NULL;
    }

  if (1 < fin.diffuse || 0 > fin.diffuse)
    {
      fprintf(stderr, "surface_create(): Illegal diffuse value %f, should be between 0 and 1.\n", fin.diffuse);
      return NULL;
    }

  s = (surface) malloc(sizeof(struct surface_t));

  if (NULL == s)
    {
      fprintf(stderr, "surface_create(): Could not allocate memory.\n");
      return NULL;
    }

  s->reflection = fin.reflection;
  s->diffuse = fin.diffuse;

  if (Color == pig.type)
    {
      s->mode = Color;
      s->col = pig.col;
    }
  else
    {
      s->mode = TextureMap;
      s->texture_name = pig.texture_name;
      s->texture = bitmap_read_ppm(s->texture_name);
      if (NULL == s->texture)
	{
	  fprintf(stderr, "surface_create(): Could not load image map %s.\n", s->texture_name);
	  free(s);
	  return NULL;
	}
    }

  return s;
}

surface_mode surface_get_mode(surface s)
{
  return s->mode;
}

color surface_get_color(surface s)
{
  if (Color != s->mode) {
    fprintf(stderr, "surface_get_color(): Surface does not have mode Color.\n");
    return DEF_BG_COLOR;
  }

  return s->col;
}


bitmap surface_get_texture_map(surface s)
{
  if (TextureMap != s->mode) {
    fprintf(stderr, "surface_get_texture_map(): Surface does not have mode TextureMap.\n");
    return NULL;
  }

  return s->texture;
}

real surface_get_reflection(surface s)
{
  return s->reflection;
}

real surface_get_diffuse(surface s)
{
   return s->diffuse;
}

struct pigment _pigment_create_color(color c)
{
  struct pigment p;

  p.type = Color;
  p.col = c;

  return p;
}

struct pigment _pigment_create_texturemap(char *name)
{
  struct pigment p;

  p.type = TextureMap;
  p.texture_name = name;

  return p;
}

struct finish _finish_create(real ref, real dif)
{
  struct finish f;

  f.reflection = ref;
  f.diffuse = dif;

  return f;
}
