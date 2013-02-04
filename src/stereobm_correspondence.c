#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "stereobm_config.h"
#include "stereobm_correspondence.h"
#include "stereobm_image.h"

struct depth_image_t stereobm_correspondence(struct g_image_t *left_image, struct g_image_t *right_image) {
	struct depth_image_t depth_map;

	memset(depth_map.pixels, 0, sizeof(depth_map.pixels));

	// Scan across the whole image, pixel by pixel.  Our goal is to find a
	// 'd' (disparity) value for every pixel in the image.  This is the
	// distance between the pixel in the left image and that same image in
	// the right.
	// TODO: Should 'x' start at SAD_WINDOW_SIZE, as we're accessing
	// 'pixels[x - d][y]'?
	for(int x = MAX_DISPARITY; x < IMG_WIDTH; x++) {
		for(int y = 0; y < IMG_HEIGHT; y++) {
			// The 'min_score' is the best 'd' value for this pixel.
			// 'min_disparity' is the disparity that corresponds to
			// that score.
			int min_score = INT_MAX, min_disparity = 0;
			for(int d = MIN_DISPARITY; d < MAX_DISPARITY; d++) {
				int score = stereobm_sad_score(left_image, right_image, x, y, d);

				// If this 'd' value is better than the current
				// best, record it.
				if(score < min_score) {
					min_score = score;
					min_disparity = d;
				}
			}

			// TODO: Normalize the map to 0..255.

			// Save the result to the output.
			depth_map.pixels[x][y].d = min_disparity;
		}
	}

	return depth_map;
}

int stereobm_sad_score(struct g_image_t *left_image, struct g_image_t *right_image, int x, int y, int d) {
	int score = 0;

	for(int dx = x - SAD_WINDOW_SIZE2 - 1; dx < x + SAD_WINDOW_SIZE2; dx++) {
		for(int dy = y - SAD_WINDOW_SIZE2 - 1; dy < y + SAD_WINDOW_SIZE2; dy++) {
			// Compute the disparity of a
			// single pixel
			int disparity = abs(left_image->pixels[dx][dy].g
					- right_image->pixels[dx - d][dy].g);

			// Add this to the total SAD
			score += disparity;
		}
	}

	return score;
}
