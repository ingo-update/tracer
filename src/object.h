#include "real.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "pgram.h"

#include "ray.h"
#include "hitdata.h"

#ifndef _OBJECT_H_
#define _OBJECT_H_

typedef enum {Sphere, Plane, Triangle, Pgram} object_type;

typedef struct object_t *object;
struct object_t
{
  union {sphere sp; plane pl; triangle tr; pgram pg;} obj;
  object_type type;
};

object object_create_sphere(sphere o);
object object_create_triangle(triangle o);
object object_create_plane(plane o);
object object_create_pgram(pgram o);

real object_hit_distance(object o, ray r);
hitdata object_hitdata(object o, ray r);

#endif /* _OBJECT_H_ */
