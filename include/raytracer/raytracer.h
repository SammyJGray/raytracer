#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "../scene/scene.h"
#include "../structures/ray.h"

Vec3 canvas_to_viewport(int x, int y);

float compute_lighting(Hit* hit, Ray* ray, Scene* scene);

RGB trace_ray(Ray* ray, Scene* scene, int depth);
#endif
