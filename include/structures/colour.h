#ifndef COLOUR_H
#define COLOUR_H

#define GREY ((RGB) {20,20,20})
#define WHITE ((RGB) {255,255,255})
#define RED ((RGB) {255,0,0})
#define GREEN ((RGB) {0,255,0})
#define BLUE ((RGB) {0,0,255})
#define YELLOW ((RGB) {255,255,0})

typedef struct {
	float r, g, b;
} RGB;

RGB colour_scale(RGB c, float s);

RGB colour_add(RGB a, RGB b);

void colour_print(RGB c);
#endif
