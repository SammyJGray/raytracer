#ifndef SCENE_H
#define SCENE_H

#include "../structures/object.h"
#include "../structures/light.h"

#define MAX_OBJECTS 8000
#define MAX_LIGHTS 100

typedef struct BVHNode {
	AABB bounds;
	struct BVHNode* left;
	struct BVHNode* right;
	int isLeaf;
	int indexes[MAX_OBJECTS];
	int len;
} BVHNode;

typedef struct {
	Object objects[MAX_OBJECTS];
	int objectsLen;

	BVHNode root;

	Light lights[MAX_LIGHTS];
	int lightsLen;
} Scene;	

void init_scene(Scene* scene);

void add_sphere(Sphere* sphere, Scene* scene, Material material);

void add_triangle(Triangle* triangle, Scene* scene, Material material);

void add_model(Triangle* triangles,Scene* scene, Material material);

void add_light(Light* light, Scene* scene);
#endif

