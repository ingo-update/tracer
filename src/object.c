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
      return sphere_hit_distance((sphere) o, r);
    case Plane:
      return plane_hit_distance((plane) o, r);
    case Triangle:
      return triangle_hit_distance((triangle) o, r);
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
      return sphere_hitdata((sphere) o, r);
    case Plane:
      return plane_hitdata((plane) o, r);
    case Triangle:
      return triangle_hitdata((triangle) o, r);
    default:
      fprintf(stderr,"object_hitdata(): Unknown object type %d.\n", t);
      exit(EXIT_FAILURE);
    }
}
