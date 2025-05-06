#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "../scene/scene.h"
#include "../structures/ray.h"

int sphere_intersect(void* data, Ray* ray, Hit* hit);

int triangle_intersect(void* data, Ray* ray, Hit* hit);

int AABB_intersect(AABB* bounds, Ray* ray);

void node_intersect(BVHNode* node, Ray* ray, Scene* scene, Hit* hit,int deep);

void closest_intersection(Ray* ray, Scene* scene, Hit* hit);

#endif
