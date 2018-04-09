#include <stdio.h>

#include "real.h"

#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct vector_t vector;

struct vector_t {
  real x, y, z;
};

/* Destructive vector macros */

#define vector_d_difference(vec, tor, diff) \
  (diff.x) = (vec.x)-(tor.x);\
  (diff.y) = (vec.y)-(tor.y);\
  (diff.z) = (vec.z)-(tor.z)

#define vector_d_dot_product(vec, tor, dp) \
  dp = (vec.x)*(tor.x)+(vec.y)*(tor.y)+(vec.z)*(tor.z)

/* Function prototypes */

vector vector_create(real x, real y, real z);
real vector_get_x (vector vec);
real vector_get_y (vector vec);
real vector_get_z (vector vec);

vector vector_xp(vector vec, vector tor);
vector vector_sum(vector vec, vector tor);
vector vector_diff(vector vec, vector tor);
vector vector_sp(vector vec, real scalar);
vector vector_norm(vector vec);

real vector_dp(vector vec, vector tor);
real vector_length(vector vec);
// int vector_compare(vector vec, vector tor);
void vector_print(FILE *os, vector vec);

#endif /* _VECTOR_H_ */

