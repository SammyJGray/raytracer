# Raytracer in C

This is a raytracer I have been creating from scratch in C. 

It can render two types of primatives, spheres and triangles. These primatives are sorted into a bounding volume heirscrchy by their central x coordinate, and are grouped together by an axis aligned bounding box.

Ambient, directional and point lights have been added. My lighting fuction calculatea the diffuse and specular reflections, as well as the shadows casted. 

My create model function currently parses through a wav object file of a teapot and creates the triangles primatives for them all.

I have created several different structures like vec3, rgb, material, etc to simplify my code.

The final image is created in a .ppm format.

Im the future I would like the add reflections, refractions, and more types of primatives to my renderer.

This has been a fun project to complete from scratch and has helped soldify both my knowledge of C and of computer graphics.
