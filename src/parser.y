%{
#include <stdlib.h>
#include <stdio.h>

#include "vector.h"
#include "color.h"
#include "world.h"
#include "camera.h"
#include "defaults.h"

  extern char *yytext;
  extern int linecount;
  extern int yylex(void);

  extern world the_world;
  extern camera the_camera;

  int yyerror(char *s);
%}

%union {
  double         value;		/* for numbers */
  char		 *string;	/* for names */
  vector	 vector;
  color          color;
  surface        surface;
  sphere         sphere;
  plane          plane;
  light          light;
  camera         camera;
  struct sph_opt sphere_opt;
  struct finish  finish;
  struct pigment pigment;
};

%token LBRACE RBRACE LANGLE RANGLE COMMA

%token <value> NUMBER
%token <string> STRING

%token NAME UNCAUGHT

%token KEY_SPHERE KEY_PIGMENT KEY_COLOR
%token KEY_FINISH KEY_TRIANGLE KEY_TRIANGLEPNT
%token KEY_DIFFUSE KEY_REFLECTION
%token KEY_RED KEY_GREEN KEY_BLUE
%token KEY_LOCATION KEY_LOOK KEY_BACKGROUND
%token KEY_CAMERA KEY_RGB KEY_SKY KEY_LIGHT
%token KEY_PGRAM KEY_PGRAMPNT
%token KEY_PLANE KEY_PLANEPNT KEY_IMAGE KEY_PPM
%token KEY_POLE KEY_EQUATOR

%type <vector> vector
%type <value> number
%type <color> color
%type <surface> object_mods
%type <sphere> sphere
%type <plane> plane
%type <triangle> triangle
%type <color> background
%type <camera> camera
%type <finish> opt_finish
%type <finish> finish
%type <value> diffuse
%type <value> opt_diffuse
%type <value> reflection
%type <value> opt_reflection
%type <pigment> opt_pigment
%type <pigment> pigment
%type <string> image
%type <sphere_opt> sphere_opt
%type <light> light

%%

scene:  item
|       item scene
;

item:	sphere { world_add_object(the_world, (object) $1);}
|       plane { world_add_object(the_world, (object) $1);}
|       background {world_put_background(the_world, $1);}
|       camera { the_camera = $1;}
|       light { world_add_light(the_world, $1);}
;

light:	KEY_LIGHT LBRACE vector RBRACE
{ $$ = light_create($3, color_create_rgb(255,255,255));}
|	KEY_LIGHT LBRACE vector color RBRACE
{ $$ = light_create($3, $4);}
;

vector: LANGLE number COMMA number COMMA number RANGLE
{ $$ = vector_create($2, $4, $6);}
;

plane:  KEY_PLANE LBRACE vector COMMA number object_mods RBRACE
{ $$ = plane_create($3, vector_sp(vector_norm($3), $5), $6);}
;

/* This is not POVray syntax, but is an easier way to specify a plane. */

plane:  KEY_PLANEPNT LBRACE vector COMMA vector object_mods RBRACE
{ $$ = plane_create($3, $5, $6);}
;

sphere:	KEY_SPHERE LBRACE vector COMMA number sphere_opt object_mods RBRACE
{ $$ = sphere_create(sph_opt_get_pole($6),
		     sph_opt_get_equator ($6), $3, $5, $7);}
;

sphere_opt: /* Empty */
{ $$ = sph_opt_create(vector_create(0,1,0),vector_create(1,0,0));}
|       KEY_POLE vector KEY_EQUATOR vector
{ $$ = sph_opt_create($2, $4);}
;

plane:
KEY_TRIANGLE LBRACE vector COMMA vector COMMA vector object_mods RBRACE
{ $$ = triangle_create($3, vector_diff($5, $3), vector_diff($7, $3), $8); }
;

/* Again, this is not POVray, but provides an alternative way of
   specifying a triangle. */

plane:
KEY_TRIANGLEPNT LBRACE vector COMMA vector COMMA vector object_mods RBRACE
{ $$ = triangle_create($3, $5, $7, $8);}
;

/* New Feature! Parallelograms use same syntax as triangle */

plane:
KEY_PGRAM LBRACE vector COMMA vector COMMA vector object_mods RBRACE
{ $$ = pgram_create($3, vector_diff($5, $3), vector_diff($7, $3), $8); }
;

plane:
KEY_PGRAMPNT LBRACE vector COMMA vector COMMA vector object_mods
RBRACE
{ $$ = pgram_create($3, $5, $7, $8);}
;

object_mods: opt_pigment opt_finish
{ $$ = surface_create($1,$2);}
;

opt_pigment: /* Empty */
{ $$ = pigment_create_color(DEF_OBJECT_COLOR);}
|       pigment
{ $$ = $1;}
;

pigment:
KEY_PIGMENT LBRACE color RBRACE
{ $$ = pigment_create_color($3);}
|       KEY_PIGMENT LBRACE image RBRACE
{ $$ = pigment_create_texturemap($3);}
;

image:	KEY_IMAGE LBRACE KEY_PPM STRING RBRACE
{ $$ = $4;}
;

opt_finish: /* Empty */
{ $$ = finish_create(DEF_REFLECTION, DEF_DIFFUSE);}
|       finish
;

finish:
KEY_FINISH LBRACE opt_diffuse opt_reflection RBRACE
{ $$ = finish_create($4, $3);}
;

opt_diffuse: /* Empty */
{ $$ = 1;}
|       diffuse
;

diffuse:
KEY_DIFFUSE number
{ $$ = $2;}
;

opt_reflection: /* Empty */
{ $$ = 0;}
|        reflection
;

reflection:
KEY_REFLECTION number
{ $$ = $2;}
;

color:  KEY_COLOR KEY_RED number KEY_GREEN number KEY_BLUE number
{ $$ = color_create_rgb((255 * $3),(255 * $5),(255 * $7));}
|       KEY_COLOR KEY_RGB vector
{ $$ =color_create_rgb(255*vector_get_x($3),
		       255*vector_get_y($3),
		       255*vector_get_z($3));}
;

camera:	KEY_CAMERA LBRACE KEY_LOCATION vector KEY_SKY vector KEY_LOOK vector
RBRACE
{ $$ = camera_create($4, $6, $8); }
;

background:
KEY_BACKGROUND LBRACE color RBRACE
{ $$ = $3; }
;

number: NUMBER
{ $$ = $1; }
;

%%

int yyerror(char *s) {
  fprintf(stderr, "yyparse(): Syntax error at '%s' on line %d\n", s, linecount);
  return 1;
}
