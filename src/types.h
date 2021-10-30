#include <float.h>

#ifndef _TYPES_H_
#define _TYPES_H_

/* What type we use for real numbers */
typedef double real;
#define REAL_MAX DBL_MAX
#define REAL_OFFSET 0.00001 /* Compensation for inaccurate arithmetics. */

typedef enum {Distance, Full} tracing_mode;
typedef enum {Sphere, Plane, Triangle, Parallelogram} object_type;

#endif /* _TYPES_H_ */
