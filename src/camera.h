#include "vector.h"

#ifndef _CAMERA_H_
#define _CAMERA_H_

typedef struct camera_t* camera;
struct camera_t
{
  vector position;
  vector direction;
  vector up;
};

camera camera_create(vector position, vector up, vector direction);
vector camera_get_direction(camera c);
vector camera_get_position(camera c);
vector camera_get_up(camera c);

#endif /* _CAMERA_H_ */
