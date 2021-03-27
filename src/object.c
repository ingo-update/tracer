#include "object.h"

real object_hit_distance(object o, ray r)
{
  object_type t;

  t = object_get_type(o);
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

  t = object_get_type(o);
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
