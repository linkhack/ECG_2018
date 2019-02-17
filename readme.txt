Author:
11728332 Patrick Link

Bonus Tasks: 
Task2:
Strafing
    - right and zAxis are the axes corresponding to up direction and right direction in camera
      coordinate system. We can add the difference of x and y of mouse in these directions to strafe and 
      add strafe to the position of the camera.
    - the roation part is the same without strafe or with strafe. Looking at 0 from position is the 
      same as looking at strafe from strafe + position
Task3:
Torus:
    - Embedding is given by x(alpha,beta) = (R+r*cos(beta))*cos(alpha)
			    y(alpha,beta) = (R+r*cos(beta))*sin(alpha)
			    z(alpha,beta) = r*sin(beta)
      where R is the big radius(tube), r is the small radius (circle), alpha is the angle along the tube 
      and beta is the angle along circle. The formula can be found on http://mathworld.wolfram.com/Torus.html
      but can be deduced using vector calculations ( vector to center of circle + vector to circle)
    - If we keep alpha fixed and vary beta we get a circle on the tube, paralel to the radius from the center
      of the torus to the center of the tube. Therefore, for two consecutive values of alpha, one step beeing
      2*pi/segmentsOnTube, we get a small tube similar to the cylinder. And similarly for two consecutive value
      of alpha and beta we get a quad. We just have to be careful at the end of each circle and at the end of the
      tube as these point connect not to the next but, but to the one at the beginning of the circle, analog to
      the case with the sphere or cylinder, we just have two seperate edge cases, namely at the end of each circle
      and at the end of the tube
Task4:
    -I implemented multiple lights by defining a uniform array of length 64 (128 would be too many registers) and having a
     uniform which saves the number of lights of one type. In the shader we just iterate over all lights, the ambient part
     is just calculated 1 time. The uniforms are all set by LightManager.cpp. We used 19 point lights, 3 directional lights, 4 Spotlights
    - For the spotlight observe that if direction is normalized then dot(direction,-lightdirection)=cos(angle between light direction and direction of light), where lightdireciton
      is the direction from the fragment point to the light ource. Thus we can interpolate using the cosine, where we have 1 if cos(angle)>cos(innerangle) hermite in between and 0 if
      cos(angle)<cos(outerAngle). We can just multiply the specular and diffuse part with this value.
    - We implemented the Disney BRDF. the lightning model is given by L = (1-mettalic)diffuse + specular + sheen + clearcloat, where we weight sheen and clearcoat by a factor between 0
      and 1. All the formulas are given in the appendix of the paper. The specular part is given by G*F*D/(4*(l.n)*(v.n), where D is the microfacet distribution function, F is the 
      Fresnel term, and D is the geometric attenuation. We can include the denominator into the D function. For G the Generalized-Trowbridge-Reitz distribution. For F we use the
      Schlick approximation, and G is given by the Smith shadowing factor G=G(l.n)G(v.n) and G depends on the chosen distribution for D. In the diffuse part they give an approximation
      for the roughness back reflection. The disney BRDF is not completely physics based, because they think it's more important that the results look good, and the change of parameters
      lead to intuitive material changes. We showed what happens if we vary roughness and metallic from 0 to 1. A mettalic surface has only the specular part.
      We also see that if we use Gouraud interpolation, the resulting shading is very wrong, because it depends strongly on the normal/halfvector.
Task 4
    -I implemented the uv coordinate fix, just as described in the task.
      

Tested on NVIDIA hardware.