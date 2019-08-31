#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

int test_datatype()
{
  int fail;
  vector vec;

  fail = 0;
  vec = vector_create(42, 17, 4711);

  if ((vector_get_x(vec) - 42) > OFFSET ||
      (vector_get_y(vec) - 17) > OFFSET ||
      (vector_get_z(vec) - 4711) > OFFSET)
    {
      fprintf(stderr, "FAIL: Vector created incorrectly: ");
      vector_print(stderr, vec);
      fprintf(stderr, " should be (42.000000, 17.000000, 4711.000000)\n");
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
      fprintf(stdout, "SUCCESS\n");
      exit(EXIT_SUCCESS);
    }
  else
    {
      fprintf(stdout, "FAILURE\n");
      exit(EXIT_FAILURE);
    }
}
