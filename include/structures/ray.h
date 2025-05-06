#ifndef RAY_H
#define RAY_H

#include "vector.h"
#include "material.h"

typedef struct {
	Vec3 origin;
	Vec3 direction;
} Ray;

typedef struct {
	float t; float min;
	Vec3 posistion;
	Vec3 normal;
	Material* material;
	int found;
} Hit;

#endif
