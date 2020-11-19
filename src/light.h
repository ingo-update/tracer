#include "vector.h"
#include "color.h"

#ifndef _LIGHT_H_
#define _LIGHT_H_

typedef struct light_t* light;

struct light_t
{
  vector location;
  color col;
};

light light_create(vector location, color col);

color light_get_color(light l);
vector light_direction(light l, vector position);

#endif /* _LIGHT_H_ */
