#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

int test_datatype()
{
  int fail;
  int x = 42;
  int y = -17;
  int z = 4711;
  vector vec;

  fail = 0;
  vec = vector_create(x, y, z);

  if ((vector_get_x(vec) != x) || (vector_get_y(vec) != y) || (vector_get_z(vec) != z))
    {
      fprintf(stderr, "FAIL: Vector created incorrectly: ");
      vector_print(stderr, vec);
      fprintf(stderr, " should be (%f, %f, %f)\n", (real) x, (real) y, (real) z);
      ++fail;
    }

  return fail;
}

int test_arithmetic()
{
  int fail;
  vector a, b, c;
  real d;

  fail = 0;
  a = vector_create(42, 0, -17);
  b = vector_create(2, 2, 2);

  c = vector_xp(a, b);
  if ((vector_get_x(c) != 34) || (vector_get_y(c) != -118) || (vector_get_z(c) != 84))
    {
      fprintf(stderr, "FAIL: Vector cross product calculated incorrectly: ");
      vector_print(stderr, c);
      fprintf(stderr, " should be (%f, %f, %f)\n", (real) 34, (real) -118, (real) 84);
      ++fail;
    }

  c = vector_sum(a, b);
  if ((vector_get_x(c) != 44) || (vector_get_y(c) != 2) || (vector_get_z(c) != -15))
    {
      fprintf(stderr, "FAIL: Vector sum calculated incorrectly: ");
      vector_print(stderr, c);
      fprintf(stderr, " should be (%f, %f, %f)\n", (real) 44, (real) 2, (real) -15);
      ++fail;
    }

  c = vector_diff(a, b);
  if ((vector_get_x(c) != 40) || (vector_get_y(c) != -2) || (vector_get_z(c) != -19))
    {
      fprintf(stderr, "FAIL: Vector difference calculated incorrectly: ");
      vector_print(stderr, c);
      fprintf(stderr, " should be (%f, %f, %f)\n", (real) 40, (real) -2, (real) -19);
      ++fail;
    }

  c = vector_sp(a, -3);
  if ((vector_get_x(c) != -126) || (vector_get_y(c) != 0) || (vector_get_z(c) != 51))
    {
      fprintf(stderr, "FAIL: Vector scalar product calculated incorrectly: ");
      vector_print(stderr, c);
      fprintf(stderr, " should be (%f, %f, %f)\n", (real) -126, (real) 0, (real) 51);
      ++fail;
    }

  c = vector_norm(a);
  if ((vector_get_x(c) - 0.926947 > OFFSET) ||
      (vector_get_y(c) - 0 > OFFSET) ||
      (vector_get_z(c) + 0.375193 > OFFSET)
      )
    {
      fprintf(stderr, "FAIL: Vector normalize calculated incorrectly: ");
      vector_print(stderr, c);
      fprintf(stderr, " should be (%f, %f, %f)\n", 0.926947, (real) 0, -0.375193);
      ++fail;
    }

  d = vector_dp(a, b);
  if (d != 50)
    {
      fprintf(stderr, "FAIL: Vector dot procuct calculated incorrectly: %f should be %f\n", d, (real) 50);
      ++fail;
    }

  d = vector_length(a);
  if (d - 50 > OFFSET)
    {
      fprintf(stderr, "FAIL: Vector length calculated incorrectly: %f should be %f\n", d, (real) 50);
      ++fail;
    }

  c = vector_create(42, 0, -17);
  if (!vector_compare(c, a))
    {
      fprintf(stderr, "FAIL: Vector campare failed: ");
      vector_print(stderr, c);
      fprintf(stderr, " should be equal to ");
      vector_print(stderr, a);
      fprintf(stderr, "\n");
      ++fail;
    }

  if (vector_compare(c, b))
    {
      fprintf(stderr, "FAIL: Vector campare failed: ");
      vector_print(stderr, c);
      fprintf(stderr, " should not be equal to ");
      vector_print(stderr, b);
      fprintf(stderr, "\n");
      ++fail;
    }

  return fail;
}

int main()
{
  int fail;

  fail = 0;

  if (0 == fail)
    {
      fail += test_datatype();
    }

  if (0 == fail)
    {
      fail += test_arithmetic();
    }

  if (0 == fail)
    {
      fprintf(stdout, "SUCCESS\n");
      exit(EXIT_SUCCESS);
    }
  else
    {
      fprintf(stdout, "FAILURE; %d tests failed.\n", fail);
      exit(EXIT_FAILURE);
    }
}
