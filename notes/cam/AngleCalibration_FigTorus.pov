
//

#include "colors.inc"
#include "stones.inc"
#include "metals.inc"

/*
// environment limits
plane
{
	<0., 1., 0.>, -1
//	pigment { checker color Red, color Blue scale 1 }
    texture {
      pigment {
        wood
        color_map {
          [0.0 color DarkTan]
          [0.9 color DarkBrown]
          [1.0 color VeryDarkBrown]
        }
        turbulence 0.5
        scale <0.2, 0.3, 1>
      }
      finish { phong 1 }
    }
}
*/
background { color rgb <1., 1., 1.> }


// illumination
light_source
{
	<2., 4., -3.> color White
	area_light <.25, 0, 0>, <0, 0, .25>, 5, 5
	adaptive 1
	jitter
}
sphere // sun
{
	<2., 4., -3.>, 1.
	pigment { color Yellow }
	no_shadow
}

// detector
box
{
	// <right, up, away>
	<0., 0., 0.>, <2., .05, 3.>
	pigment { checker color Black, color Gray75 scale .05 }
}

// camera
camera
{
	location <2.25, 2.0, -2.>
	look_at <1.00, 0., 1.0>
	angle 45
}

// objects

cylinder
{
	<1.00, 0.00, 1.50>,
	<1.00, 0.70, 1.50>,
	.01
	pigment { color Blue }
//	texture { T_Brass_1A scale 4 }
//	texture { T_Chrome_5E }
}

torus
{
	.25, .25
	rotate 60.*x
	rotate 90.*z
	translate 0.90*x
	translate 1.70*z
//	translate 0.00*y
//	pigment { color Yellow }
	texture { T_Chrome_5C }
//	texture { T_Brass_1A scale 4 }
}
/*
*/


/*
sphere
{
	<0., 1., 2.>, 2.
	texture { T_Chrome_5E }
}
*/


/*
cone
{
	<0., 1., -1.>, 0.01
	<1., 2., 3.>, .0
	texture { T_Brass_1A scale 4 }
	rotate y*45
}
*/

