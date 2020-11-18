#include "types.h"

#include "sphere.h"
#include "plane.h"

#include "ray.h"
#include "hitdata.h"

#ifndef _OBJECT_H_
#define _OBJECT_H_

typedef struct object_t *object;

struct object_t
{
  object_type type;
};

real object_hit_distance(object o, ray r);
hitdata object_hitdata(object o, ray r);

#endif /* _OBJECT_H_ */
