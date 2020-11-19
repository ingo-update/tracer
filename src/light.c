#include <stdlib.h>

#include "light.h"

light light_create(vector loc, color col) {

  light new;

  new = (light) malloc(sizeof(struct light_t));
  if (NULL==new)
    {
      fprintf(stderr,"light_create(): Could not allocate light.\n");
      exit(-1);
    }

  new->location = loc;
  new->col = col;

  return(new);
}

color light_get_color(light l)
{
  return(l->col);
}

vector light_direction(light l, vector point)
{
  return(vector_diff((l->location), point));
}
