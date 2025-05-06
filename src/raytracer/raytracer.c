#include "../../include/raytracer/raytracer.h"
#include "../../include/image.h"
#include "../../include/raytracer/intersection.h"

#include <math.h>


// This adjusts the coordinates such that the ray cast from pixel 0,0 is being cast from -WIDTH/2,-HEIGHT/2  in the 3d scene
inline Vec3 canvas_to_viewport(int x, int y){
	return (Vec3) {(x-WIDTH/2.0)*1.0/WIDTH,(y-HEIGHT/2.0)*1.0/WIDTH,1.0};
}

// Finds the colour of each pixel
RGB trace_ray(Ray* ray, Scene* scene, int depth){
	Hit hit = {.found=-1,.t=INFINITY,.min=1};
	closest_intersection(ray,scene,&hit);
	if (hit.found == -1) return (RGB) {255,255,255};

	float i = compute_lighting(&hit,ray,scene);
	RGB local_colour = colour_scale(hit.material->colour,i);

	float reflective = hit.material->reflective;
	if (depth<=0||reflective<=0) return local_colour;

	return local_colour;
}
