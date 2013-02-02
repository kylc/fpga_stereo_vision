#include <stdlib.h>
#include <limits.h>

#include "stereobm_config.h"
#include "stereobm_correspondence.h"
#include "stereobm_image.h"

struct depth_image_t stereobm_correspondence(struct g_image_t *left_image, struct g_image_t *right_image) {
	struct depth_image_t depth_map;

	// Scan across the whole image, pixel by pixel.  Our goal is to find a
	// 'd' (disparity) value for every pixel in the image.  This is the
	// distance between the pixel in the left image and that same image in
	// the right.
	// TODO: Should 'x' start at SAD_WINDOW_SIZE, as we're accessing
	// 'pixels[x - d][y]'?
	for(int x = 0; x < IMG_WIDTH; x++) {
		for(int y = 0; y < IMG_HEIGHT; y++) {
			// The 'min_score' is the best 'd' value for this pixel.
			// 'min_disparity' is the disparity that corresponds to
			// that score.
			int min_score = INT_MAX, min_disparity = 0;
			for(int d = MIN_DISPARITY; d < MAX_DISPARITY; d++) {
				int score = 0;

				// TODO: This actually computes with a window
				// size of 2 * SAD_WINDOW_SIZE?
				for(int dx = x - SAD_WINDOW_SIZE; dx < x + SAD_WINDOW_SIZE; dx++) {
					for(int dy = y - SAD_WINDOW_SIZE; dy < y + SAD_WINDOW_SIZE; dy++) {
						// Compute the disparity of a
						// single pixel
						int disparity = abs(left_image->pixels[dx][dy].g
									- right_image->pixels[dx - d][dy].g);

						// Add this to the total SAD
						score += disparity;
					}
				}

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
