#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "vector.h"
#include "colour.h"
#include "scene.h"
#include "ray.h"

Vec3 canvas_to_viewport(int x, int y);

int sphere_intersect(void* data, Ray* ray, Hit* hit);

int triangle_intersect(void* data, Ray* ray, Hit* hit);

RGB trace_ray(Ray* ray, Scene* scene,int depth);

#endif
