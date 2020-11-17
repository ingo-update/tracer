#include <stdio.h>
#include <math.h>

#include "vector.h"

vector vector_xp(vector vec, vector tor)
{
  return vector_create(vector_get_y(vec) * vector_get_z(tor) - vector_get_y(tor) * vector_get_z(vec),
		       vector_get_z(vec) * vector_get_x(tor) - vector_get_z(tor) * vector_get_x(vec),
		       vector_get_x(vec) * vector_get_y(tor) - vector_get_x(tor) * vector_get_y(vec)
		       );
}

vector vector_sum(vector vec, vector tor)
{
  return vector_create(vector_get_x(vec) + vector_get_x(tor),
		       vector_get_y(vec) + vector_get_y(tor),
		       vector_get_z(vec) + vector_get_z(tor)
		       );
}

vector vector_diff(vector vec, vector tor)
{
  return vector_create(vector_get_x(vec) - vector_get_x(tor),
		       vector_get_y(vec) - vector_get_y(tor),
		       vector_get_z(vec) - vector_get_z(tor)
		       );
}

vector vector_sp(vector vec, real scalar)
{
  return vector_create(vector_get_x(vec) * scalar, vector_get_y(vec) * scalar, vector_get_z(vec) * scalar);
}

vector vector_norm(vector vec)
{
  return vector_sp(vec, (1 / vector_length(vec)));
}

real vector_dp(vector vec, vector tor)
{
  return vector_get_x(vec) * vector_get_x(tor) + vector_get_y(vec) * vector_get_y(tor) + vector_get_z(vec) * vector_get_z(tor);
}

real vector_length(vector vec)
{
  return sqrt(vector_dp(vec, vec));
}

int vector_compare(vector vec, vector tor)
{
  return (vector_get_x(vec) == vector_get_x(tor)) && (vector_get_y(vec) == vector_get_y(tor)) && (vector_get_z(vec) == vector_get_z(tor));
}

void vector_print(FILE *os, vector vec)
{
  fprintf(os, "(%f, %f, %f)", vector_get_x(vec), vector_get_y(vec), vector_get_z(vec));
}
