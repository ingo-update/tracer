#include <stdio.h>
#include <stdlib.h>

#include "real.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"

#include "hitdata.h"

object_type object_get_type(object o) {
  return(o->type);
}

static object _new_object() {

  object new = malloc(sizeof(struct object_t));

  if (NULL == new) {
    fprintf(stderr,"_new_object(): Could not allocate object.\n");
  }

  return(new);
}

object object_create_sphere(sphere o) {

  object b;

  b = _new_object();
  b->obj.sp = o;
  b->type = Sphere;

  return(b);
}

object object_create_triangle(triangle t) {

  object b;

  b = _new_object();
  b->obj.tr = t;
  b->type = Triangle;

  return(b);
}

object object_create_plane(plane o) {

  object b;

  b = _new_object();
  b->obj.pl = o;
  b->type = Plane;

  return(b);
}

real object_hit_distance(object o, ray r)
{
  object_type t;

  t = o->type;
  switch (t)
    {
    case Sphere:
      return sphere_hit_distance(o->obj.sp, r);
    case Plane:
      return plane_hit_distance(o->obj.pl, r);
    case Triangle:
      return triangle_hit_distance(o->obj.tr, r);
    default:
      return NO_HIT;
    }
}

hitdata object_hitdata(object o, ray r)
{
  object_type t;

  t = o->type;
  switch (t)
    {
    case Sphere:
      return sphere_hitdata(o->obj.sp, r);
    case Plane:
      return plane_hitdata(o->obj.pl, r);
    case Triangle:
      return triangle_hitdata(o->obj.tr, r);
    default:
      fprintf(stderr,"object_hitdata(): Unknown object type.\n");
      exit(EXIT_FAILURE);
    }
}
