#include "../../include/raytracer/lighting.h"

#include "../../include/structures/vector.h"
#include "../../include/raytracer/intersection.h"
#include "../../include/structures/light.h"

#include <math.h>

// Calculates the intenisty of each point based on the lighting in the scene
float compute_lighting(Hit* hit, Ray* ray, Scene* scene){

	float intensity = 0.0; Vec3 direction; float normal_dot_direction;
	Vec3 reflected; float reflected_dot_view;

	for (int i = 0; i < scene->lightsLen; i++){

		if (scene->lights[i].type == AMBIENT) intensity += scene->lights[i].intensity;
		
		else {
			if (scene->lights[i].type == DIRECTION) direction = scene->lights[i].direction;
			if (scene->lights[i].type == POINT) direction = vec3_sub(scene->lights[i].posistion,hit->posistion);

			// To calculate shadows, we do an intersection test with a ray posistioned at the point and in the direction of the light
			//  If an intersection is found, than we return without adding lighting
			Hit shadowHit = {.found=-1,.t=INFINITY,.min=0.001};
			Ray shadowRay = {.origin=hit->posistion,.direction=direction};
			closest_intersection(&shadowRay,scene,&shadowHit);
			if (shadowHit.found != -1) continue;

			// -- Ambient lighting --
			normal_dot_direction = vec3_dot(hit->normal,direction);
			if (normal_dot_direction > 0) intensity += scene->lights[i].intensity * normal_dot_direction / (vec3_magnitude(hit->normal)*vec3_magnitude(direction));
			
			// -- Specular lighting --
			if (hit->material->specular != -1){
				reflected = vec3_sub(vec3_scale(2*normal_dot_direction,hit->normal),direction);
				Vec3 view = vec3_scale(-1,ray->direction);
				reflected_dot_view = vec3_dot(reflected,view);
				if (reflected_dot_view > 0){
					intensity += scene->lights[i].intensity * pow(reflected_dot_view/(vec3_magnitude(reflected)*vec3_magnitude(view)),hit->material->specular);
				}
			}
		}
	}
	return intensity;
}
