#include "color.h"
#include "light.h"
#include "object.h"
#include "ray.h"
#include "hitdata.h"
#include "options.h"

#ifndef _WORLD_H_
#define _WORLD_H_

/* Lists are private to the world module */
struct list {
  void *car;
  struct list *cdr;
};

struct world_t {
   color bg_color;
   char *bg_map_name;
   struct list *objects;
   struct list *lights;
};

typedef struct world_t* world;

color world_look(world w, ray r, unsigned int depth, shading_mode s);
hitdata world_hit(world w, ray r);

world world_create();
void world_add_object(world w, object o);
void world_add_light(world w, light l);
void world_put_background(world w, color col);
color world_get_background(world w);
void world_put_background_map_name(world w, char *map_name);
char *world_get_background_map_name(world w);

#endif
