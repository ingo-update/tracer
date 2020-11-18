/* Hi, Emacs!  Please pretend this file is written in -*-C-*- */

%{
#include <string.h>
#include "world.h"
#include "vector.h"
#include "color.h"
#include "sphere.h"
#include "plane.h"
#include "camera.h"

#include "parser.h"

int linecount = 1;
%}

digit	[0-9]
number	"-"?{digit}+("."{digit}+)?
name	[_a-zA-Z]+

%%

"//"[^\n]*\n /* eat comments */

background	return KEY_BACKGROUND;
blue		return KEY_BLUE;
camera		return KEY_CAMERA;
color		return KEY_COLOR;
colour		return KEY_COLOR;
diffuse		return KEY_DIFFUSE;
equator		return KEY_EQUATOR;
finish		return KEY_FINISH;
green		return KEY_GREEN;
image_map	return KEY_IMAGE;
light_source	return KEY_LIGHT;
location	return KEY_LOCATION;
look_at		return KEY_LOOK;
pgram		return KEY_PGRAM;
pgram_point	return KEY_PGRAMPNT;
pigment		return KEY_PIGMENT;
plane		return KEY_PLANE;
plane_point	return KEY_PLANEPNT;
pole		return KEY_POLE;
ppm		return KEY_PPM;
red		return KEY_RED;
reflection	return KEY_REFLECTION;
rgb		return KEY_RGB;
sky		return KEY_SKY;
sphere		return KEY_SPHERE;
triangle	return KEY_TRIANGLE;
triangle_point	return KEY_TRIANGLEPNT;

"{"		return LBRACE;
"}"		return RBRACE;
"<"		return LANGLE;
">"		return RANGLE;
","		return COMMA;

{number}	{ yylval.value = atof(yytext); return NUMBER; }
\"[^\"]*\"	{ yylval.string = strdup(yytext+1);
		  yylval.string[strlen(yytext+1)-1] = '\0';
	          return STRING;
		}

{name}		return NAME;
\n 		{ linecount++; }
[ \t\r]		;

.		return UNCAUGHT;
%%
