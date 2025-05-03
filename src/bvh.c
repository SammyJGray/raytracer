#include "../include/bvh.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

AABB merge_bounds(AABB* b1, AABB* b2){
	AABB b3;

	b3.min.x = fminf(b1->min.x,b2->min.x);	
	b3.min.y = fminf(b1->min.y,b2->min.y);	
	b3.min.z = fminf(b1->min.z,b2->min.z);	
	
	b3.max.x = fmaxf(b1->max.x,b2->max.x);	
	b3.max.y = fmaxf(b1->max.y,b2->max.y);	
	b3.max.z = fmaxf(b1->max.z,b2->max.z);

	return b3;
}

int compare_x(const void* a, const void* b){
	Object* aObject = (Object*) a; Object* bObject = (Object*) b;
	return aObject->bounds.center.x - bObject->bounds.center.x;
}

void add_node(BVHNode* node, int* indexes, int objectsLen, Scene* scene){
	int index;
	AABB bounds = {.min=(Vec3){INFINITY,INFINITY,INFINITY},.max=(Vec3){-INFINITY,-INFINITY,-INFINITY}};

	for (int i =0; i < objectsLen; i++){
		index = indexes[i];
		node->indexes[i] = index;
		bounds = merge_bounds(&bounds,&scene->objects[index].bounds);
	}
	node->len = objectsLen;
	node->bounds = bounds;

	if (node->len < 10){
		node->isLeaf = 1;
		return;
	}
	int leftLen = node->len / 2; int rightLen = node->len - leftLen;
	node->left = (BVHNode*)malloc(sizeof(BVHNode));	
	node->right = (BVHNode*)malloc(sizeof(BVHNode));

	add_node(node->left,node->indexes,leftLen,scene);
	add_node(node->right,node->indexes+leftLen,rightLen,scene);
}

void free_node(BVHNode* node){
	if (node->isLeaf != 1){
		free_node(node->left);
		free_node(node->right);
	}
	free(node);
}

void free_bvh(Scene* scene){
	if (scene->objectsLen < 10) return;
	free_node(scene->root.left);
	free_node(scene->root.right);
}

void init_bvh(Scene* scene){
	AABB bounds;

	qsort(scene->objects,scene->objectsLen,sizeof(Object),compare_x);

	int indexes[scene->objectsLen];
	for (int i = 0; i < scene->objectsLen; i++){
		indexes[i] = i;
	}
	
	add_node(&scene->root,indexes,scene->objectsLen,scene);
}
