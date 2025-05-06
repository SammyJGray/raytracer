#include "../../include/scene/scene.h"
#include "../../include/raytracer/intersection.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

void init_scene(Scene* scene){
	scene->objectsLen = 0;
}

void sphere_bounds(Sphere* sphere, AABB* bounds){
	bounds->min.x = sphere->center.x - sphere->radius;
	bounds->min.y = sphere->center.y - sphere->radius;
	bounds->min.z = sphere->center.z - sphere->radius;
	
	bounds->max.x = sphere->center.x + sphere->radius;
	bounds->max.y = sphere->center.y + sphere->radius;
	bounds->max.z = sphere->center.z + sphere->radius;

	bounds->center.x = (bounds->min.x + bounds->max.x) / 2.0;
	bounds->center.y = (bounds->min.y + bounds->max.y) / 2.0;
	bounds->center.z = (bounds->min.z + bounds->max.z) / 2.0;
}

void add_sphere(Sphere* sphere, Scene* scene, Material material){
	scene->objects[scene->objectsLen].material = material;
	sphere_bounds(sphere,&scene->objects[scene->objectsLen].bounds);
	scene->objects[scene->objectsLen].data = sphere;
	scene->objects[scene->objectsLen].intersect = sphere_intersect; 
	scene->objectsLen++;
}

void triangle_bounds(Triangle* triangle, AABB* bounds){
	bounds->min.x = fminf(fminf(triangle->p0.x,triangle->p1.x),triangle->p2.x);		
	bounds->min.y = fminf(fminf(triangle->p0.y,triangle->p1.y),triangle->p2.y);		
	bounds->min.z = fminf(fminf(triangle->p0.z,triangle->p1.z),triangle->p2.z);		
	
	bounds->max.x = fmaxf(fmaxf(triangle->p0.x,triangle->p1.x),triangle->p2.x);		
	bounds->max.y = fmaxf(fmaxf(triangle->p0.y,triangle->p1.y),triangle->p2.y);		
	bounds->max.z = fmaxf(fmaxf(triangle->p0.z,triangle->p1.z),triangle->p2.z);		
	
	bounds->center.x = (bounds->min.x + bounds->max.x) / 2.0;
	bounds->center.y = (bounds->min.y + bounds->max.y) / 2.0;
	bounds->center.z = (bounds->min.z + bounds->max.z) / 2.0;
}

void add_triangle(Triangle* triangle, Scene* scene, Material material){
	Vec3 edge1 = vec3_sub(triangle->p1,triangle->p0);
	Vec3 edge2 = vec3_sub(triangle->p2,triangle->p0);
	triangle->normal = vec3_normalize(vec3_cross(edge1,edge2));

	scene->objects[scene->objectsLen].material = material;
	triangle_bounds(triangle,&scene->objects[scene->objectsLen].bounds);
	scene->objects[scene->objectsLen].data = triangle;
	scene->objects[scene->objectsLen].intersect = triangle_intersect;
	scene->objectsLen++;
}

void add_model(Triangle* triangles,Scene* scene,Material material){
	FILE *file = fopen("models/teapot.obj","r");

	char line[256];
	Vec3 vertices[5000];
	int vCount = 0; int fCount = 0;

	while(fgets(line,sizeof(line),file)){
		if (strncmp(line,"v ",2)==0){
			sscanf(line+2,"%f %f %f",&vertices[vCount].x,&vertices[vCount].y,&vertices[vCount].z);
			vertices[vCount] = vec3_scale(0.5,vertices[vCount]);
			vertices[vCount].z += 5;
			vCount++;
		}
		else if (strncmp(line,"f ",2)==0){
			int p0,p1,p2;
			sscanf(line+2,"%d %d %d",&p0,&p1,&p2);
			triangles[fCount].p0 = vertices[p0-1]; triangles[fCount].p1 = vertices[p1-1]; triangles[fCount].p2 = vertices[p2-1];
			add_triangle(&triangles[fCount],scene,material);
			fCount++;
		}
	}
}


void add_light(Light* light, Scene* scene){
	scene->lights[scene->lightsLen] = *light;
	scene->lightsLen++;
}
