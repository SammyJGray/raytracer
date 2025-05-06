#include "../../include/scene/tree.h"

#include <math.h>
#include <stdlib.h>

// Merges two AABB's together
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

// Returns a negative value if B has larger x, positive if A has larger x, and zero if equal
int compare_x(const void* a, const void* b){
	Object* aObject = (Object*) a; Object* bObject = (Object*) b;
	return aObject->bounds.center.x - bObject->bounds.center.x;
}

// Recursively adds nodes to the bounding volume heirarchy tree
void add_node(BVHNode* node, int* indexes, int objectsLen, Scene* scene){
	int index;
	AABB bounds = {.min=(Vec3){INFINITY,INFINITY,INFINITY},.max=(Vec3){-INFINITY,-INFINITY,-INFINITY}};

	// Inititalizes the nodes value based on the paramters provided
	// Merges the AABB of every index at the same time to find the AABB of the group of indexes
	for (int i =0; i < objectsLen; i++){
		index = indexes[i];
		node->indexes[i] = index;
		bounds = merge_bounds(&bounds,&scene->objects[index].bounds);
	}
	node->len = objectsLen;
	node->bounds = bounds;

	// If the node now only contains 10 primatives or less, the node is a leaf node and the function returns
	if (node->len < 10){
		node->isLeaf = 1;
		return;
	}

	//Allocate memory for the children, and pass one half of the indexes to each child
	int leftLen = node->len / 2; int rightLen = node->len - leftLen;
	node->left = (BVHNode*)malloc(sizeof(BVHNode));	
	node->right = (BVHNode*)malloc(sizeof(BVHNode));

	add_node(node->left,node->indexes,leftLen,scene);
	add_node(node->right,node->indexes+leftLen,rightLen,scene);
}

// Recursively free tree nodes
void free_node(BVHNode* node){
	if (node->isLeaf != 1){
		free_node(node->left);
		free_node(node->right);
	}
	free(node);
}

// Frees the bounding veiw heirarchy tree
void free_bvh(Scene* scene){
	if (scene->objectsLen < 10) return;
	free_node(scene->root.left);
	free_node(scene->root.right);
}

// Creates the bounding veiw heirarchy
void init_bvh(Scene* scene){
	AABB bounds;

	// Sorts indexes by their x coordinate so that when the index are recursively split in half, they are grouped next to each other
	qsort(scene->objects,scene->objectsLen,sizeof(Object),compare_x);

	int indexes[scene->objectsLen];
	for (int i = 0; i < scene->objectsLen; i++){
		indexes[i] = i;
	}
	
	add_node(&scene->root,indexes,scene->objectsLen,scene);
}
