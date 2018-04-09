#include <stdlib.h>
#include <stdio.h>

#include "vector.h"
#include "camera.h"

camera camera_create(vector position, vector up, vector direction) {

  camera c;

  c = (camera) malloc(sizeof(struct camera_t));

  if (NULL==c) {
    fprintf(stderr,"camera_create(): Could not allocate memory\n");
    return NULL;
  }

  c->position = position;
  c->up = up;
  c->direction = direction;

  return c;
}

vector camera_get_direction(camera c)
{
  return c->direction;
}

vector camera_get_position(camera c)
{
  return c->position;
}

vector camera_get_up(camera c)
{
  return c->up;
}
