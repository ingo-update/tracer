#include <stdlib.h>
#include <stdio.h>

#include "trace.h"

bitmap trace(world w, camera cam, options opt)
{
  bitmap bmp;
  unsigned int x,y;
  real rx, ry;
  vector a, b, c;
  vector cam_pos, target_center, target_right, target_down, target_zz;
  vector target_x, target_y, ray_dir;
  ray r;

  bmp = bitmap_create(options_get_pict_pix_width(opt), options_get_pict_pix_height(opt));
  if (NULL == bmp)
    {
      fprintf(stderr, "trace(): Could not allocate bitmap.\n");
      return NULL;
    }

  cam_pos = camera_get_position(cam);

  /* Set up target base vectors */
  target_center = vector_norm(vector_diff(camera_get_direction(cam), cam_pos));
  target_right = vector_norm(vector_xp(camera_get_up(cam), target_center));
  target_down = vector_norm(vector_xp(target_right, target_center));

  /* Top left corner of target (ZeroZero) */
  a = vector_sp(target_center, options_get_zoom(opt));
  b = vector_sp(target_right, options_get_pict_real_width(opt) / 2);
  c = vector_sp(target_down, options_get_pict_real_height(opt) / 2);
  target_zz = vector_sum(cam_pos, vector_diff(vector_diff(a, b), c));

  rx = options_get_rx(opt);
  ry = options_get_ry(opt);

  for (y = 0 ; y < options_get_pict_pix_height(opt) ; ++y)
    {
      target_y = vector_sp(target_down, ry * (real) y);
      for (x = 0 ; x < options_get_pict_pix_width(opt) ; ++x)
	{
	  target_x = vector_sp(target_right, rx * (real) x);
	  ray_dir = vector_norm(vector_diff(vector_sum(target_zz, vector_sum(target_x, target_y)), cam_pos));

	  r = ray_create(cam_pos, ray_dir);
	  bitmap_put_pixel(bmp, x, y, world_look(w, r, options_get_reflection_depth(opt), options_get_shading(opt)));
	}
    }

  return bmp;
}
