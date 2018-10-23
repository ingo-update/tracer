#include <stdio.h>
#include <stdlib.h>

#include "surface.h"

surface surface_create(struct pigment pig, struct finish fin)
{
  surface s;

  if (1 < fin.reflection)
    {
      fprintf(stderr, "surface_create(): Reflection value %f is greater than 1 (100%%)\n", fin.reflection);
      return NULL;
    }

  if (1 < fin.diffuse)
    {
      fprintf(stderr, "surface_create(): Diffuse value %f is greater than 1 (100%%)\n", fin.diffuse);
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
    exit(EXIT_FAILURE);
  }

  return s->col;
}


bitmap surface_get_texture_map(surface s)
{
  if (TextureMap != s->mode) {
    fprintf(stderr, "surface_get_texture_map(): Surface does not have mode TextureMap.\n");
    exit(EXIT_FAILURE);
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
