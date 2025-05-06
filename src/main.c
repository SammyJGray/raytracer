#include "../include/image.h"
#include "../include/raytracer/raytracer.h"
#include "../include/scene/scene.h"
#include "../include/structures/vector.h"
#include "../include/scene/tree.h"
#include "../include/structures/material.h"

#include <stdlib.h>

int main() {
	int image[WIDTH][HEIGHT][3];
	init_buffer(image,GREY);

	// -- SCENE SETUP --
	Scene scene;
	init_scene(&scene);

	Material material = {.colour=GREEN,.specular=50,.reflective=0.3};

	Triangle* triangles = (Triangle*)malloc(MAX_OBJECTS*sizeof(Triangle));
	add_model(triangles,&scene,material);

	material = (Material){.colour=RED,.specular=500,.reflective=0.2};
	Sphere sphere = {(Vec3){0,-1,3},1};
	add_sphere(&sphere,&scene,material);

	material = (Material){.colour=BLUE,.specular=500,.reflective=0.3};
	Sphere sphere2 = {(Vec3){2,0,4},1};
	add_sphere(&sphere2,&scene,material);

	material = (Material){.colour=GREEN,.specular=10,.reflective=0.4};
	Sphere sphere3 = {(Vec3){-2,0,4},1};
	add_sphere(&sphere3,&scene,material);

	material = (Material){.colour=YELLOW,.specular=1000,.reflective=0.5};
	Sphere sphere4 = {(Vec3){0,-5001,0},5000};
	add_sphere(&sphere4,&scene,material);

	init_bvh(&scene);

	Light lightAmbient = {.type=AMBIENT,.intensity=0.2};
	add_light(&lightAmbient,&scene);
	
	Light lightPoint = {.type=POINT,.intensity=0.6,.posistion=(Vec3){2,1,0}};
	add_light(&lightPoint,&scene);

	Light lightDirection = {.type=DIRECTION,.intensity=0.2,.direction=(Vec3){1,4,4}};
	add_light(&lightDirection,&scene);
	// -- SCENE SETUP END --

	// Cast ray for each pixel in the image
	RGB colour; Ray ray;
	for (int x = 0; x < WIDTH; x++){
		for (int y = 0; y < HEIGHT; y++){
			ray.direction = vec3_normalize(vec3_sub(canvas_to_viewport(x,y),ray.origin));
			colour = trace_ray(&ray,&scene,3);
			put_pixel(image,x,y,colour);
		}
	}

	// Free allocated memory
	free_bvh(&scene);
	free(triangles);

	// Write image
	write_buffer_to_PPM(image);
	return 0;
}	
