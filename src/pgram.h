#include "vector.h"
#include "surface.h"
#include "ray.h"
#include "hitdata.h"

#ifndef _PGRAM_H_
#define _PGRAM_H_

typedef struct pgram_t* pgram;
struct pgram_t {
  vector corner0;
  vector a_leg;
  vector another_leg;
  surface surf;
};

pgram pgram_create(vector c0, vector a, vector b, surface surf);
vector pgram_get_corner0(pgram obj);
vector pgram_get_a_leg(pgram obj);
vector pgram_get_another_leg(pgram obj);
surface pgram_get_surface(pgram obj);

real pgram_hit_distance(pgram o, ray r);
hitdata pgram_hitdata(pgram o, ray r);

#endif /* _PGRAM_H_ */
