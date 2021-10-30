// Test file for tracer.

sphere {<0,0,0>, 30000 pigment {color rgb <0,0,0>} finish {reflection 0}} // universe limits

// White sphere
sphere {<0,0,0>, 150 pigment {color rgb <1,1,1>} finish {diffuse 1 reflection 0.2}}

// Colored spheres
sphere {<0,300,0>,     100 pigment {color rgb <1,0,0>} finish {diffuse 1 reflection 0.2}}
sphere {<-260,150,0>,  100 pigment {color rgb <1,1,0>} finish {diffuse 1 reflection 0.2}}
sphere {<-260,-150,0>, 100 pigment {color rgb <0,1,0>} finish {diffuse 1 reflection 0.2}}
sphere {<0,-300,0>,    100 pigment {color rgb <0,1,1>} finish {diffuse 1 reflection 0.2}}
sphere {<260,-150,0>,  100 pigment {color rgb <0,0,1>} finish {diffuse 1 reflection 0.2}}
sphere {<260,150,0>,   100 pigment {color rgb <1,0,1>} finish {diffuse 1 reflection 0.2}}

// Texturemapped reflecting triangle (ascii ppm)
triangle {<-500,-500,400>, <500,-500,0>, <-500,500,0>
          pigment {image_map {ppm "build/test/test_asc.ppm"}}
          finish {diffuse 1 reflection 0.5}
          }

// Texturemapped sphere (binary ppm)
sphere {<0,0,700>, 500
        pigment {image_map {ppm "build/test/checkers.ppm"}}
        finish {reflection 0.3}
        }

// Let's have a tilted background plane
plane_point {<0,1,1>, <0,0,5000> pigment {color rgb <1,0.7,0.7>}
	finish {diffuse 1 reflection 0.5}}

// Main Light
light_source {<-1000,0,-1000> color rgb <0.7,0.7,0.7>}

// Backlight
light_source {<0,0,-1200> color rgb <0.3,0.3,0.3>}

// We could use a camera as well
camera {location <0,0,-1200> sky <0,1,0> look_at <0,0,0>}
