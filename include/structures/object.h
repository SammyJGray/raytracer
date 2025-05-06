#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"

typedef int (*normalFunc)(Ray* ray);
typedef int (*intersectionFunc)(void* object, Ray* ray, Hit* hit);

typedef enum {
	SPHERE,
	TRIANGLE
} ObjectType;

typedef struct {
	Vec3 center;
	float radius;
} Sphere;

typedef struct {
	Vec3 p0,p1,p2;
	Vec3 normal;
} Triangle;

typedef struct {
	Vec3 min, max, center;
} AABB;

typedef struct {
	ObjectType type;
	void* data;
	AABB bounds;
	Material material;
	intersectionFunc intersect;
	normalFunc get_normal;
} Object;

#endif
