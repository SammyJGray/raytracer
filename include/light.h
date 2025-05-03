#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

typedef enum {
	AMBIENT,
	DIRECTION,
	POINT
} LightType;

typedef struct {
	LightType type;
	float intensity;
	Vec3 direction;
	Vec3 posistion;
} Light;

#endif
