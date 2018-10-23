#include <stdio.h>
#include <math.h>

#include "vector.h"

vector vector_create(real x, real y, real z)
{
  vector new;

  (new.x) = x;
  (new.y) = y;
  (new.z) = z;

  return new;
}

real vector_get_x (vector vec)
{
  return vec.x;
}

real vector_get_y (vector vec)
{
  return vec.y;
}

real vector_get_z (vector vec)
{
  return vec.z;
}

vector vector_xp(vector vec, vector tor)
{
  vector cross;

  cross.x = vec.y * tor.z - vec.z * tor.y;
  cross.y = vec.z * tor.x - vec.x * tor.z;
  cross.z = vec.x * tor.y - vec.y * tor.x;

  return cross ;
}

vector vector_sum(vector vec, vector tor)
{
  vector sum;

  sum.x = vec.x + tor.x;
  sum.y = vec.y + tor.y;
  sum.z = vec.z + tor.z;

  return sum;
}

vector vector_diff(vector vec, vector tor)
{
  vector diff;

  diff.x = vec.x - tor.x;
  diff.y = vec.y - tor.y;
  diff.z = vec.z - tor.z;

  return diff;
}

vector vector_sp(vector vec, real scalar)
{
  vector product;

  product.x = vec.x * scalar;
  product.y = vec.y * scalar;
  product.z = vec.z * scalar;

  return product;
}

vector vector_norm(vector vec)
{
  return vector_sp(vec, (1 / vector_length(vec)));
}

real vector_dp(vector vec, vector tor)
{
  return vec.x * tor.x + vec.y * tor.y + vec.z * tor.z;
}

real vector_length(vector vec)
{
  return sqrt(vector_dp(vec, vec));
}

void vector_print(FILE *os, vector vec)
{
  fprintf(os, "(%f, %f, %f)", vec.x, vec.y, vec.z);
}
