// Testfil till tracer av Ingemar Åberg

// ./tracer -x512 -y512 -w1 < test2.pov

sphere {<0,0,0>, 30000 pigment {color rgb <0,0,0>}} // universe limits

// White sphere
sphere {<0,0,0>, 150 pigment {color rgb <1,1,1>} finish {reflection 0.2}}

sphere {<300,0,0>, 100 pigment {color rgb <1,0,0>} finish {reflection 0.2}}
sphere {<0,300,0>, 100 pigment {color rgb <0,0,1>} finish {reflection 0.2}}
sphere {<-300,0,0>, 100 pigment {color rgb <0,1,0>} finish {reflection 0.2}}
sphere {<0,-300,0>, 100 pigment {color rgb <1,1,0>} finish {reflection 0.2}}

// Really large and shiny cyan sphere behind the others
sphere {<0,0,700>, 500
        pigment {color rgb <0,1,1>}
        finish {reflection 0.5}
        }

// Main Light
light_source {<0,1000,-1000> color rgb <0.7,0.7,0.7>}

// Backlight
light_source {<0,0,-1200> color rgb <0.3,0.3,0.3>}

// Texturemapped reflecting triangle
triangle {<-500,500,400>, <500,500,0>, <-500,-500,0>
          pigment {image_map {ppm "build/test/test_asc.ppm"}}
          finish {reflection 0.5}
          }

// Let's have a tilted background plane
plane_point {<1,0,1>, <0,0,5000> pigment {color rgb <1,0.7,0.7>}
	finish {reflection 0.5}}

// We could use a camera as well
camera {location <0,0,-1200> sky <1,0,0> look_at <0,0,0>}
