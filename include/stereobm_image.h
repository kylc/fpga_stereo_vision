#ifndef _STEREOBM_IMAGE_H_
#define _STEREOBM_IMAGE_H_

#include "stereobm_config.h"

struct rgb8_t {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct g8_t {
	unsigned char g;
};

struct depth8_t {
	unsigned char d;
};

struct rgb_image_t {
	struct rgb8_t pixels[IMG_WIDTH][IMG_HEIGHT];
};

struct g_image_t {
	struct g8_t pixels[IMG_WIDTH][IMG_HEIGHT];
};

struct depth_image_t {
	struct depth8_t pixels[IMG_WIDTH][IMG_HEIGHT];
};

#endif
