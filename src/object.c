#include <stdio.h>
#include <stdlib.h>

#include "object.h"

object_type object_get_type(object o)
{
  return o->type;
}

real object_hit_distance(object o, ray r)
{
  object_type t;

  t = o->type;
  switch (t)
    {
    case Sphere:
      return (sphere_hitdata((sphere) o, r, Distance)).distance;
    case Plane:
    case Triangle:
    case Parallelogram:
      return (plane_hitdata((plane) o, r, Distance)).distance;
    default:
      fprintf(stderr,"object_hit_distance(): Unknown object type %d.\n", t);
      exit(EXIT_FAILURE);
    }
}

hitdata object_hitdata(object o, ray r)
{
  object_type t;

  t = o->type;
  switch (t)
    {
    case Sphere:
      return sphere_hitdata((sphere) o, r, Full);
    case Plane:
    case Triangle:
    case Parallelogram:
      return plane_hitdata((plane) o, r, Full);
    default:
      fprintf(stderr,"object_hitdata(): Unknown object type %d.\n", t);
      exit(EXIT_FAILURE);
    }
}
