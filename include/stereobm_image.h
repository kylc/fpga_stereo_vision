#ifndef _STEREOBM_IMAGE_H_
#define _STEREOBM_IMAGE_H_

#include <stdint.h>

#include "stereobm_config.h"

struct rgb8_t {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct g8_t {
	uint8_t g;
};

struct depth8_t {
	uint8_t d;
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
