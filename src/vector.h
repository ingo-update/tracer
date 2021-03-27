#include <stdio.h>
#include <math.h>

#include "types.h"

#ifndef _VECTOR_H_
#define _VECTOR_H_

#define vector_create(x, y, z) (vector) {x, y, z}
#define vector_get_x(vec) vec.x
#define vector_get_y(vec) vec.y
#define vector_get_z(vec) vec.z

typedef struct vector_t vector;

struct vector_t {
  real x, y, z;
};

/* Function prototypes */

vector vector_xp(vector vec, vector tor);
vector vector_sum(vector vec, vector tor);
vector vector_diff(vector vec, vector tor);
vector vector_sp(vector vec, real scalar);
vector vector_norm(vector vec);

real vector_dp(vector vec, vector tor);
real vector_length(vector vec);
int vector_compare(vector vec, vector tor);
void vector_print(FILE *os, vector vec);

#endif /* _VECTOR_H_ */
