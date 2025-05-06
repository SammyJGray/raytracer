#ifndef BVH_H
#define BVH_H

#include "scene.h"

AABB merge_bounds(AABB* b1, AABB* b2);

void init_bvh(Scene* scene);

void free_bvh(Scene* scene);
#endif
