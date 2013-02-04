#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "stereobm_config.h"
#include "stereobm_correspondence.h"
#include "stereobm_image.h"

struct depth_image_t stereobm_correspondence(struct g_image_t *left_image, struct g_image_t *right_image) {
	struct depth_image_t depth_map;

	// Initialize the map to all black (unknown) pixels.
	// TODO: HLS does not support memset.
	// memset(depth_map.pixels, 0, sizeof(depth_map.pixels));

	// Scan across the whole image, pixel by pixel.  Our goal is to find a
	// 'd' (disparity) value for every pixel in the image.  This is the
	// distance between the pixel in the left image and that same image in
	// the right.  Note that we start our scan at 'MAX_DISPARITY' as we
	// won't be able to shift left 'MAX_DISPARITY' pixels for our disparity
	// loop otherwise.
	for(int x = MAX_DISPARITY; x < IMG_WIDTH; x++) {
		for(int y = 0; y < IMG_HEIGHT; y++) {
			// The 'min_score' is the best 'd' value for this pixel.
			// 'min_disparity' is the disparity that corresponds to
			// that score.
			unsigned int min_score = INT_MAX;
			for(int d = MIN_DISPARITY; d < MAX_DISPARITY; d++) {
				int score = stereobm_sad_score(left_image, right_image, x, y, d);

				// If this 'd' value is better than the current
				// best, record it.
				if(score < min_score) {
					min_score = score;

					// Save the result to the output.
					depth_map.pixels[x][y].d = d;
				}
			}
		}
	}

	return depth_map;
}

int stereobm_sad_score(struct g_image_t *left_image, struct g_image_t *right_image, int x, int y, int d) {
	unsigned int score = 0;

	for(int dx = -SAD_WINDOW_SIZE2; dx < SAD_WINDOW_SIZE2; dx++) {
		for(int dy = -SAD_WINDOW_SIZE2; dy < SAD_WINDOW_SIZE2; dy++) {
			// Compute the disparity of a single pixel
			unsigned char disparity = abs(left_image->pixels[x + dx][y + dy].g
							- right_image->pixels[x + dx - d][y + dy].g);

			// Add this to the total SAD
			score += disparity;
		}
	}

	return score;
}
