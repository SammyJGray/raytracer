#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
	float x; float y; float z;
} Vec3;

Vec3 vec3_sub(Vec3 u, Vec3 v);

Vec3 vec3_add(Vec3 u, Vec3 v);

Vec3 vec3_scale(float s, Vec3 u);

Vec3 vec3_scale_add(Vec3 u, float s, Vec3 v);

float vec3_dot(Vec3 u, Vec3 v);

Vec3 vec3_cross(Vec3 u, Vec3 v);

Vec3 vec3_normalize(Vec3 u);

float vec3_magnitude(Vec3 u);

void vec3_print(Vec3 u);
#endif
