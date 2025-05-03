#include "../include/raytracer.h"
#include "../include/vector.h"
#include "../include/image.h"

#include <math.h>
#include <stdio.h>

inline Vec3 canvas_to_viewport(int x, int y){
	return (Vec3) {(x-WIDTH/2.0)*1.0/WIDTH,(y-HEIGHT/2.0)*1.0/WIDTH,1.0};
}

int sphere_intersect(void* data, Ray* ray, Hit* hit){
	Sphere* sphere = (Sphere*) data;
	Vec3 originToSphere = vec3_sub(ray->origin,sphere->center);

	float a = vec3_dot(ray->direction,ray->direction);
	float b = 2.0f * vec3_dot(originToSphere, ray->direction);
	float c = vec3_dot(originToSphere,originToSphere) - sphere->radius * sphere->radius;

	float discriminant = (b*b) - (4*a*c);
	if (discriminant < 0) return 0;
	float sqrtDisc = sqrtf(discriminant);
	float t = (-b-sqrtDisc) / (2.0*a);

	if (t<0){
		t = (-b+sqrtDisc) / (2.0*a);

		if (t<0) return 0;
	}

	if (t > hit->t || t < hit->min) return 0;
	hit->found = 1;
	hit->t = t;
	hit->posistion = vec3_scale_add(ray->origin,hit->t,ray->direction);
	hit->normal = vec3_normalize(vec3_sub(hit->posistion,sphere->center));
	return 1;	
}

int triangle_intersect(void* data, Ray* ray, Hit* hit){
	Triangle* triangle = (Triangle*) data;
	float epsilon = 1e-4;

	Vec3 edge1 = vec3_sub(triangle->p1,triangle->p0);
	Vec3 edge2 = vec3_sub(triangle->p2,triangle->p0);

	Vec3 p = vec3_cross(ray->direction,edge2);
	float determinant = vec3_dot(edge1,p);

	if (fabs(determinant) < epsilon) return 0;
	Vec3 originToPoint = vec3_sub(ray->origin,triangle->p0);
	float inverse_determinant = 1.0 / determinant;
	


	float u = inverse_determinant * vec3_dot(originToPoint,p);
	if (u<0.0||u>1.0) return 0;

	Vec3 q = vec3_cross(originToPoint,edge1);

	float v = inverse_determinant * vec3_dot(ray->direction,q);
	if (v<0.0||u+v>1.0) return 0;

	float t = inverse_determinant * vec3_dot(edge2,q);
	if (t < epsilon || t > hit->t) return 0;
	
	hit->found = 1;
	hit->t = t;
	hit->posistion = vec3_scale_add(ray->origin,hit->t,ray->direction);
	hit->normal = triangle->normal;
	return 1;
}

int AABB_intersect(AABB* bounds, Ray* ray){
	float tMin = -INFINITY; float tMax = INFINITY;
	float origin, direction, minBound, maxBound;
	float t1,t2,smaller,bigger;

	for (int i = 0; i < 3; i++){
		origin = ((float*)&ray->origin)[i];
		direction = ((float*)&ray->direction)[i];
		minBound = ((float*)&bounds->min)[i];
		maxBound = ((float*)&bounds->max)[i];
		
		if (direction != 0){
			t1 = (minBound-origin) / direction;
			t2 = (maxBound-origin) / direction;

			smaller = fminf(t1,t2);
			bigger = fmaxf(t1,t2);
			
			tMin = fmaxf(tMin,smaller);
			tMax = fminf(tMax,bigger);

			if (tMax<=tMin){
				return 0;
			}
		}

		else if (origin<minBound || origin>maxBound) return 0;
	}
	return 1;
}

void node_intersect(BVHNode* node, Ray* ray, Scene* scene, Hit* hit,int deep){
	if (AABB_intersect(&node->bounds,ray) == 0) return;
	
	if (node->isLeaf == 1){
		int index;
		for (int i = 0; i < node->len; i++){
			index = node->indexes[i];
			if (scene->objects[index].intersect(scene->objects[index].data,ray,hit)) hit->material = &scene->objects[index].material;
		} return;
	}
	
	node_intersect(node->left,ray,scene,hit,deep+1);
	node_intersect(node->right,ray,scene,hit,deep+1);
}


int closest_intersection(Ray* ray, Scene* scene, Hit* hit){
	node_intersect(&scene->root,ray,scene,hit,0);
	hit->normal = vec3_normalize(hit->normal);
	return -1;			
}

float compute_lighting(Hit* hit, Ray* ray, Scene* scene){
	float i = 0.5;
	Vec3 lightDirection = {1,2,2};

	float intensity = 0.0; Vec3 direction; float normal_dot_direction;
	Vec3 reflected; float reflected_dot_view;
	for (int i = 0; i < scene->lightsLen; i++){
		if (scene->lights[i].type == AMBIENT) intensity += scene->lights[i].intensity;
		else {
			if (scene->lights[i].type == DIRECTION) direction = scene->lights[i].direction;
			if (scene->lights[i].type == POINT) direction = vec3_sub(scene->lights[i].posistion,hit->posistion);

			Hit shadowHit = {.found=-1,.t=INFINITY,.min=0.001};
			Ray shadowRay = {.origin=hit->posistion,.direction=direction};
			closest_intersection(&shadowRay,scene,&shadowHit);
			if (shadowHit.found != -1) continue;

			normal_dot_direction = vec3_dot(hit->normal,direction);
			if (normal_dot_direction > 0) intensity += scene->lights[i].intensity * normal_dot_direction / (vec3_magnitude(hit->normal)*vec3_magnitude(direction));
			
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

RGB trace_ray(Ray* ray, Scene* scene, int depth){
	Hit hit = {.found=-1,.t=INFINITY,.min=1};
	closest_intersection(ray,scene,&hit);
	if (hit.found == -1) return (RGB) {255,255,255};

	float i = compute_lighting(&hit,ray,scene);
	RGB local_colour = colour_scale(hit.material->colour,i);

	return local_colour;
}
