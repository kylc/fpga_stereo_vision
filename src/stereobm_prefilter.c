#include <stdint.h>
#include <math.h>

#include "stereobm_config.h"
#include "stereobm_image.h"
#include "stereobm_prefilter.h"

/* Accepts a rectified RGB image, converts it to grayscale and applies a Sobel
 * convolution.
 */
struct g_image_t stereobm_prefilter(struct rgb_image_t *image) {
	struct g_image_t grayscale = stereobm_convert_to_grayscale(image);
	struct g_image_t sobel;

	// grayscale = stereobm_sobel(&grayscale);
	stereobm_sobel(&grayscale, &sobel);

	return sobel;
}

/* Convert an RGB image to grayscale using the RGB average. */
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
void stereobm_sobel(struct g_image_t *image, struct g_image_t *output) {
	// We will create a 3x3 window centered at P = (x, y) and shift down
	// each iteration.
	uint8_t buf[2][3];
#pragma HLS ARRAY_PARTITION variable=buf complete dim=0

	for(uint16_t x = 1; x < IMG_WIDTH - 1; x++) {
		for(uint16_t y = 1; y < IMG_HEIGHT - 1; y++) {
#pragma HLS PIPELINE
			// Shift each row up
			for(uint8_t i = 0; i < 2; i++) {
#pragma HLS UNROLL
				buf[i][0] = buf[i][1];
				buf[i][1] = buf[i][2];
			}

			// Shift in new bottom row.  We don't need the center
			// column.  Omitting it allows us to fit our reads into
			// a 2P RAM every cycle.
			buf[0][2] = image->pixels[x - 1][y].g;
			buf[1][2] = image->pixels[x + 1][y].g;

			// Compute the convolution.
			int16_t d0 = buf[1][0] - buf[0][0];
			int16_t d1 = buf[1][1] - buf[0][1];
			int16_t d2 = buf[1][2] - buf[0][2];

			int16_t value = d0 + (2 * d1) + d2 + (SOBEL_CLAMP / 2);

			// Clamp the value to the new range 0..2**SOBEL_DEPTH-1.
			if(value < 0) {
				value = 0;
			} else if(value > SOBEL_CLAMP) {
				value = SOBEL_CLAMP;
			}

			output->pixels[x][y].g = value;
		}
	}
}
