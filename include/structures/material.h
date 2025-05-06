#ifndef MATERIAL_H
#define MATERIAL_H

#include "colour.h"

typedef struct {
	RGB colour;
	float specular;
	float reflective;
} Material;

#endif
