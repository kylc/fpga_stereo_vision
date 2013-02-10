#include <stdint.h>
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
	
	int32_t score_buffer[SAD_WINDOW_SIZE];

	// Scan across the whole image, pixel by pixel.  Our goal is to find a
	// 'd' (disparity) value for every pixel in the image.  This is the
	// distance between the pixel in the left image and that same image in
	// the right.  Note that we start our scan at 'MAX_DISPARITY' as we
	// won't be able to shift left 'MAX_DISPARITY' pixels for our disparity
	// loop otherwise.
	
	int32_t min_scores[IMG_WIDTH][IMG_HEIGHT];

	for(uint16_t x = 0; x < IMG_WIDTH; x++) {
		for(uint16_t y = 0; y < IMG_HEIGHT; y++) {
			min_scores[x][y] = INT32_MAX;
		}
	}

	for(uint8_t d = MIN_DISPARITY; d < MAX_DISPARITY; d++) {
		for(uint16_t y = 0; y < IMG_HEIGHT; y++) {
			for(uint16_t x = MAX_DISPARITY; x < IMG_WIDTH; x++) {
				int32_t score = x == MAX_DISPARITY ?
					stereobm_sad_score(left_image, right_image, x, y, d, score_buffer)
					: stereobm_progressive_sad_score(left_image, right_image, x, y, d, score_buffer);

				// If this 'd' value is better than the current
				// best, record it.
				if(score < min_scores[x][y]) {
					min_scores[x][y] = score;

					// Save the result to the output.
					depth_map.pixels[x][y].d = d;
				}
			}
		}
	}

	return depth_map;
}

int32_t stereobm_sad_score(struct g_image_t *left_image, struct g_image_t *right_image, uint16_t x, uint16_t y, uint8_t d, int32_t score_buffer[SAD_WINDOW_SIZE]) {
	int32_t score = 0;

	for(int dx = -SAD_WINDOW_SIZE2; dx < SAD_WINDOW_SIZE2; dx++) {
		int32_t col_score = 0;

		for(int dy = -SAD_WINDOW_SIZE2; dy < SAD_WINDOW_SIZE2; dy++) {
			// Compute the disparity of a single pixel
			uint8_t disparity = abs(left_image->pixels[x + dx][y + dy].g
							- right_image->pixels[x + dx - d][y + dy].g);

			// Add this to the total SAD
			col_score += disparity;
		}

		score += col_score;
		score_buffer[dx + SAD_WINDOW_SIZE2] = col_score;
	}

	return score;
}

int32_t stereobm_progressive_sad_score(struct g_image_t *left_image, struct g_image_t *right_image, uint16_t x, uint16_t y, uint8_t d, int32_t score_buffer[SAD_WINDOW_SIZE]) {
	for(int i = 0; i < SAD_WINDOW_SIZE - 1; i++) {
		score_buffer[i] = score_buffer[i + 1];
	}

	score_buffer[SAD_WINDOW_SIZE - 1] = 0;
	for(int dy = -SAD_WINDOW_SIZE2; dy < SAD_WINDOW_SIZE2; dy++) {
		uint8_t disparity = abs(left_image->pixels[x + SAD_WINDOW_SIZE2][y + dy].g
				- right_image->pixels[x + SAD_WINDOW_SIZE2 - d][y + dy].g);

		score_buffer[SAD_WINDOW_SIZE - 1] += disparity;
	}

	int32_t score = 0;
	for(int i = 0; i < SAD_WINDOW_SIZE; i++) {
		score += score_buffer[i];
	}

	return score;
}
