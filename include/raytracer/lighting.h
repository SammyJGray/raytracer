#ifndef LIGHTING_H
#define LIGHTING_H

#include "../structures/ray.h"
#include "../scene/scene.h"

float compute_lighting(Hit* hit, Ray* ray, Scene* scene);

#endif
