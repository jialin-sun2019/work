#ifndef _ROTATE_H_
#define _ROTATE_H_

#include<stdint.h>
#include<iostream>
#include<stdlib.h>

void** malloc_mem(size_t size) {
	unsigned char** img;
	img = (unsigned char**)malloc(sizeof(unsigned char*) * size);
	for (unsigned char i = 0; i < size; i++) {
		*(img + i) = (unsigned char*)malloc(sizeof(unsigned char) * size);
	}
	return (void**)img;
}
void display_image(unsigned char** image1, unsigned char** image2, size_t size) {
	printf("\n");
	for (uint8_t i = 0; i < size; i++) {
		printf(" |  ");
		for (uint8_t j = 0; j < size; j++) {
			printf("%02d ", *(*(image1 + i) + j));
		}
		printf(" |  ");
		for (uint8_t j = 0; j < size; j++) {
			printf("%02d ", *(*(image2 + i) + j));
		}
		printf(" |  ");
		printf("\n");
	}
}
void compare_image(unsigned char** image1, unsigned char** image2, size_t size) {
	printf("\n");
	for (uint8_t i = 0; i < size; i++) {
		printf(" |  ");
		for (uint8_t j = 0; j < size; j++) {
			printf("%02d ", *(*(image1 + i) + j));
		}
		printf(" |  ");
		for (uint8_t j = 0; j < size; j++) {
			if (*(*(image2 + i) + j) != *(*(image1 + i) + j)) {
				printf("\033[1;31m%02d \033[1;0m", *(*(image2 + i) + j));
			}
			else
			{
				printf("%02d ", *(*(image2 + i) + j));
			}
			
		}
		printf(" |  ");
		printf("\n");
	}
}
void init_image(unsigned char** image, size_t size) {
	for (uint8_t i = 0; i < size; i++) {
		for (uint8_t j = 0; j < size; j++) {
			*(*(image + i) + j) = i + 1;
		}
	}
}

void rotate1_090(unsigned char** image_i, unsigned char** image_o, size_t size) {
	for (uint8_t i = 0; i < size; i++) {
		for (uint8_t j = 0; j < size; j++) {
			*(*(image_o + i) + j) = *(*(image_i + (size - 1 - j)) + i);
		}
	}
}
void rotate1_180(unsigned char** image_i, unsigned char** image_o, size_t size) {
	for (uint8_t i = 0; i < size; i++) {
		for (uint8_t j = 0; j < size; j++) {
			*(*(image_o + i) + j) = *(*(image_i + (size - 1 - i)) + (size - 1 - j));
		}
	}
}
void rotate1_270(unsigned char** image_i, unsigned char** image_o, size_t size) {
	for (uint8_t i = 0; i < size; i++) {
		for (uint8_t j = 0; j < size; j++) {
			*(*(image_o + i) + j) = *(*(image_i + j) + (size - 1 - i));
		}
	}
}

void rotate2_090(unsigned char** image_i, unsigned char** image_o, size_t size) {
	for (size_t i = 0; i < size; i++) {
		memcpy(*(image_o + i), *(image_i + i), sizeof(unsigned char) * size);
	}
	size_t size_1 = size - 1;
	size_t size_n = size >> 1; // size/2
	unsigned char temp = 0;
	/*
					(i     ,j)
		  ________________________________
		 |                                |
		 |                                |
	(j,i)|                                | (j, size_1-i)
		 |                                |
		 |                                |
		 |________________________________|
					(size_1,j)
	*/
	for (size_t i = 0; i < size_n; i++) { // loop
		for (size_t j = 0; j < (size - i * 2); j++) { // loop-size
			unsigned char temp = image_i[i][j + i]; // [loop-size][loop]
			image_o[i][j + i] = image_i[size_1 - j - i][i];
			image_o[j + i][i] = image_i[size_1 - i][j + i];
			image_o[size_1 - i][j + i] = image_i[size_1 - j - i][size_1 - i];
			image_o[j + i][size_1 - i] = temp;
		}
	}
}

#endif // !_ROTATE_H_
