Author:
11728332 Patrick Link

Bonus Tasks: 
Strafing
    - right and zAxis are the axes corresponding to up direction and right direction in camera
      coordinate system. We can add the difference of x and y of mouse in these directions to strafe and 
      add strafe to the position of the camera.
    - the roation part is the same without strafe or with strafe. Looking at 0 from position is the 
      same as looking at strafe from strafe + position
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
      

Tested on NVIDIA hardware.