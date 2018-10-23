#include <stdlib.h>

#include "light.h"

light light_create(vector loc, color col) {

  light new = (light) malloc(sizeof(struct light_t));

  if (NULL==new) {
    fprintf(stderr,"light_create(): Kunde inte allokera objekt\n");
    exit(-1);
  }

  new->location = loc;
  new->col = col;

  return(new);
}

/*
vector light_get_location(light l) {
  return(l->location);
}
*/

color light_get_color(light l)
{
  return(l->col);
}

vector light_direction(light l, vector point)
{
  return(vector_diff((l->location),point));
}
