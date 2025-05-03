#include "../include/vector.h"

#include <stdio.h>
#include <math.h>

inline Vec3 vec3_sub(Vec3 u, Vec3 v){
	return (Vec3) {u.x-v.x, u.y-v.y, u.z-v.z};
}

inline Vec3 vec3_add(Vec3 u, Vec3 v){
	return (Vec3) {u.x+v.x,u.y+v.y,u.z+v.z};
}

inline Vec3 vec3_scale(float s, Vec3 u){
	return (Vec3) {s*u.x,s*u.y,s*u.z};
}

inline Vec3 vec3_scale_add(Vec3 u, float s, Vec3 v){
	return (Vec3) {u.x+s*v.x,u.y+s*v.y,u.z+s*v.z};
}

inline float vec3_dot(Vec3 u, Vec3 v){
	return  u.x*v.x + u.y*v.y + u.z*v.z;
}

inline Vec3 vec3_cross(Vec3 u, Vec3 v){
	return (Vec3) {u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x};
}

inline Vec3 vec3_normalize(Vec3 u){
	float len = sqrtf(vec3_dot(u,u));
	return (Vec3) {u.x/len+0.0001,u.y/len+0.0001,u.z/len+0.0001};
}

inline float vec3_magnitude(Vec3 u){
	return sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
}

void vec3_print(Vec3 u){
	printf("x: %f y: %f z: %f\n",u.x,u.y,u.z);
}
