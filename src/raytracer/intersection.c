#include "../../include/raytracer/intersection.h"

#include <math.h>

// Calculates the point of intersection between and ray and a sphere, and updates if closer than the current intersection
// Uses the quadratic forumula to find the points of intersection
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


// Calculates the point of intersection between the ray and a sphere, and updates if closer than the current intersection
// Uses the moller-trumbone intersection algorithm
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


// Checks if a ray intersects with an axis alligned bounding box
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

// Recursively checks if a ray intersects with a node and it's children until it doesn't or it reaches the leaf node
void node_intersect(BVHNode* node, Ray* ray, Scene* scene, Hit* hit,int deep){
	// If no intersection then return
	if (AABB_intersect(&node->bounds,ray) == 0) return;
	
	// If leaf node than check intersection between every primative
	if (node->isLeaf == 1){
		int index;
		for (int i = 0; i < node->len; i++){
			index = node->indexes[i];
			if (scene->objects[index].intersect(scene->objects[index].data,ray,hit)){
				// Save the material of the closest intersection
				hit->material = &scene->objects[index].material;
			}
		} return;
	}
	
	// Check intersections of the node's children
	node_intersect(node->left,ray,scene,hit,deep+1);
	node_intersect(node->right,ray,scene,hit,deep+1);
}


// Finds the closest intersection between a ray and a primative
// We only normalize the vector at the end to save time spent normalizing vector that were not the closest
void closest_intersection(Ray* ray, Scene* scene, Hit* hit){
	node_intersect(&scene->root,ray,scene,hit,0);
	hit->normal = vec3_normalize(hit->normal);
	return;			
}
