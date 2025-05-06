#include "../../include/structures/colour.h"

#include <stdio.h>

inline RGB colour_scale(RGB c, float s){
	return (RGB) {s*c.r,s*c.g,s*c.b};
}

RGB colour_add(RGB a, RGB b){
	RGB c = {a.r+b.r,a.g+b.g,a.b+b.b};
	if (c.r>255) c.r=255;
	if (c.g>255) c.g=255;
	if (c.b>255) c.b=255;
	return c;
}

void colour_print(RGB c){
	printf("Red: %f Blue: %f Green: %f\n",c.r,c.b,c.g);
}
