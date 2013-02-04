#include <math.h>

#include "stereobm_config.h"
#include "stereobm_image.h"
#include "stereobm_prefilter.h"

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

			// Take the RGB average and assign it to the 'gray'
			// value.
			grayscale.pixels[x][y].g = (pixel.r + pixel.g + pixel.b) / 3;
		}
	}

	return grayscale;
}

/* Applies a Sobel filter to the image */
struct g_image_t stereobm_sobel(struct g_image_t *image) {
	struct g_image_t sobel;

	for(int x = 1; x < IMG_WIDTH - 1; x++) {
		for(int y = 1; y < IMG_HEIGHT - 1; y++) {
			// Compute the matrix product.
			int d0 = image->pixels[x + 1][y - 1].g - image->pixels[x - 1][y - 1].g;
			int d1 = image->pixels[x + 1][y].g - image->pixels[x - 1][y].g;
			int d2 = image->pixels[x + 1][y + 1].g - image->pixels[x - 1][y + 1].g;

			int value = d0 + (2 * d1) + d2 + (SOBEL_CLAMP / 2);

			// Clamp the value to the new range 0..2**SOBEL_DEPTH-1.
			if(value < 0) {
				value = 0;
			} else if(value > SOBEL_CLAMP) {
				value = SOBEL_CLAMP;
			}

			sobel.pixels[x][y].g = value;
		}
	}

	return sobel;
}
