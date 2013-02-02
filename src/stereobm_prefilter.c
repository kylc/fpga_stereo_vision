#include <math.h>

#include "stereobm_config.h"
#include "stereobm_image.h"
#include "stereobm_prefilter.h"

int sobel_kernel_x[3][3] = {
	{ -1, 0, 1 },
	{ -2, 0, 2 },
	{ -1, 0, 1 }
};

int sobel_kernel_y[3][3] = {
	{  1,  2,  1 },
	{  0,  0,  0 },
	{ -1, -2, -1 }
};

struct g_image_t stereobm_prefilter(struct rgb_image_t *image) {
	struct g_image_t grayscale = stereobm_convert_to_grayscale(image);

	grayscale = stereobm_sobel(&grayscale);

	return grayscale;
}

/* Convert an RGB image to grayscale using the RGB average */
struct g_image_t stereobm_convert_to_grayscale(struct rgb_image_t *image) {
	struct g_image_t grayscale;

	for(int x = 0; x < IMG_WIDTH; x++) {
		for(int y = 0; y < IMG_HEIGHT; y++) {
			struct rgb8_t pixel = image->pixels[x][y];
			int value = (pixel.r + pixel.g + pixel.b) / 3;
			grayscale.pixels[x][y].g = value;
		}
	}

	return grayscale;
}

/* Applies a Sobel filter to the image */
struct g_image_t stereobm_sobel(struct g_image_t *image) {
	struct g_image_t sobel;

	for(int x = 1; x < IMG_WIDTH - 1; x++) {
		for(int y = 1; y < IMG_HEIGHT - 1; y++) {
			int s_x = sobel_kernel_x[0][0] * image->pixels[x - 1][y - 1].g
				+ sobel_kernel_x[0][1] * image->pixels[x][y - 1].g
				+ sobel_kernel_x[0][2] * image->pixels[x + 1][y - 1].g
				+ sobel_kernel_x[1][0] * image->pixels[x - 1][y].g
				+ sobel_kernel_x[1][1] * image->pixels[x][y].g
				+ sobel_kernel_x[1][2] * image->pixels[x + 1][y].g
				+ sobel_kernel_x[2][0] * image->pixels[x - 1][y + 1].g
				+ sobel_kernel_x[2][1] * image->pixels[x][y + 1].g
				+ sobel_kernel_x[2][2] * image->pixels[x + 1][y + 1].g;

			int s_y = sobel_kernel_y[0][0] * image->pixels[x - 1][y - 1].g
				+ sobel_kernel_y[0][1] * image->pixels[x - 1][y].g
				+ sobel_kernel_y[0][2] * image->pixels[x - 1][y + 1].g
				+ sobel_kernel_y[1][0] * image->pixels[x][y - 1].g
				+ sobel_kernel_y[1][1] * image->pixels[x][y].g
				+ sobel_kernel_y[1][2] * image->pixels[x][y + 1].g
				+ sobel_kernel_y[2][0] * image->pixels[x + 1][y - 1].g
				+ sobel_kernel_y[2][1] * image->pixels[x + 1][y].g
				+ sobel_kernel_y[2][2] * image->pixels[x + 1][y + 1].g;

			int value = sqrt(pow(s_x, 2) + pow(s_y, 2));

			if(value < 0) {
				value = 0;
			} else if(value > 255) {
				value = 255;
			}

			sobel.pixels[x][y].g = value;
		}
	}

	return sobel;
}
