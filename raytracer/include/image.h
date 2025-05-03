#ifndef IMAGE_H
#define IMAGE_H

#include "vector.h"
#include "colour.h"

#define WIDTH 512
#define HEIGHT 512

void init_buffer(int image[HEIGHT][WIDTH][3], RGB colour);

void put_pixel(int image[HEIGHT][WIDTH][3],int x, int y, RGB colour);

int write_buffer_to_PPM(int image[HEIGHT][WIDTH][3]);
#endif
