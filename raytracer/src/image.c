#include "../include/image.h"

#include <stdio.h>

void init_buffer(int image[HEIGHT][WIDTH][3], RGB colour){
	for (int y = 0; y < WIDTH; y++){
		for (int x = 0; x < HEIGHT; x++){	
			image[y][x][0] = colour.r;
			image[y][x][1] = colour.g;
			image[y][x][2] = colour.b;
		}
	}
}

void put_pixel(int image[HEIGHT][WIDTH][3],int x, int y, RGB colour){
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT){
		perror("Pos out of bounds!!");
		return;
	}
	
	y = HEIGHT-y-1;
	image[y][x][0] = colour.r;
	image[y][x][1] = colour.g;
	image[y][x][2] = colour.b;
}

int write_buffer_to_PPM(int image[HEIGHT][WIDTH][3]){
		
	FILE *f = fopen("images/image.ppm", "w");
	if (!f){
		perror("Error opening file!!\n");
		return 1;
	}
	
	//Formats image to P3 (plain text mode)
	fprintf(f, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

	for (int y = 0; y < WIDTH; y++){
		for (int x = 0; x < HEIGHT; x++){
			fprintf(f, "%d %d %d ", image[y][x][0], image[y][x][1], image[y][x][2]);
	  	}
	}
	
	fclose(f);
	printf("Image created!!\n");
	return 0;
}
