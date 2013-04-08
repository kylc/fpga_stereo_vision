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
	
	// The score buffer slides across the image from left to right and
	// stores the column scores.  It is used to calculate a sliding SAD
	// score, so the entire SAD window does not need to be recomputed for
	// each pixel.
	int32_t score_buffer[SAD_WINDOW_SIZE];

	// The score of the previous window.  This is used for the sliding
	// window SAD score calculation.
	int32_t prev_score = 0;

	// Stores the current minimum score for each pixel.  This must be
	// declared outside the loop as we are processing every pixel of the
	// image, then moving over to the next disparity.
	int32_t min_scores[IMG_WIDTH][IMG_HEIGHT];
	int32_t sec_min_scores[IMG_WIDTH][IMG_HEIGHT];

	// Initialize the 'min_scores' array to very high values, so we can
	// easily overwrite them with better scores.
	for(uint16_t x = 0; x < IMG_WIDTH; x++) {
		for(uint16_t y = 0; y < IMG_HEIGHT; y++) {
			min_scores[x][y] = INT32_MAX;
			sec_min_scores[x][y] = INT32_MAX;
		}
	}

	// Scan across the whole image from left to right, top to bottom.  Our
	// goal is to find a disparity ('d') value for every pixel in the image.
	// This is the spatial distance between the pixels in the left and right
	// images.
	for(uint8_t d = MIN_DISPARITY; d < MAX_DISPARITY; d++) {
		for(uint16_t y = 0; y < IMG_HEIGHT; y++) {
			for(uint16_t x = MAX_DISPARITY; x < IMG_WIDTH; x++) {
#pragma HLS PIPELINE

				// int32_t score = x == MAX_DISPARITY ?
				// 	stereobm_sad_score(left_image, right_image, x, y, d, score_buffer)
				//	: stereobm_progressive_sad_score(left_image, right_image, x, y, d, score_buffer, prev_score);

				// TODO: For now, forget about filling the
				// initial buffer.  This messes with the left
				// 'SAD_WINDOW_SIZE' columns, but makes
				// optimization much easier.
				int32_t score = stereobm_progressive_sad_score(left_image, right_image, x, y, d, score_buffer, prev_score);

				// If this score value is better than the
				// current best, record it.
				if(score < min_scores[x][y]) {
					sec_min_scores[x][y] = min_scores[x][y];
					min_scores[x][y] = score;

					// Save the result to the output.
					depth_map.pixels[x][y].d = d;
				}

				prev_score = score;
			}

		}
	}

	// Uniqueness ratio post-processing.  If the best SAD value is not a
	// certain ratio less than every other SAD value, then we discard the
	// value.
	for(uint16_t y = 0; y < IMG_HEIGHT; y++) {
		for(uint16_t x = MAX_DISPARITY; x < IMG_WIDTH; x++) {
			int thresh = min_scores[x][y] + (min_scores[x][y] * UNIQ_RATIO / 100);

			// TODO: Only do the following if this SAD value is at
			// least three disparities away, in either direction.
			if(sec_min_scores[x][y] <= thresh) {
				depth_map.pixels[x][y].d = 0;
			}
		}
	}

	return depth_map;
}

/* Computes the sum of the absolute differences of each pixel in the right image
 * and its corresponding pixel in the left image.  The 'score_buffer' is also
 * filled with the scores of each column, as they are computed.
 */
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

/* Compute the sub of absolute differences progressively.  Rather than
 * recomputing the entire SAD window, intelligently score the window based on
 * previous scores and the next column.  Given a filled 'score_buffer', this
 * will produce bit-perfect output when compared to 'stereobm_sad_score'.
 */
int32_t stereobm_progressive_sad_score(struct g_image_t *left_image, struct g_image_t *right_image, uint16_t x, uint16_t y, uint8_t d, int32_t score_buffer[SAD_WINDOW_SIZE], int32_t prev_score) {
	// The score of the row we are shifting out of the buffer.
	int32_t last_row_score = score_buffer[0];

	// Shift the buffer to the left, making room for a new column on the
	// right.
	for(int i = 0; i < SAD_WINDOW_SIZE - 1; i++) {
#pragma HLS UNROLL
		score_buffer[i] = score_buffer[i + 1];
	}

	// Calculate the new column's score.
	score_buffer[SAD_WINDOW_SIZE - 1] = 0;
	for(int dy = -SAD_WINDOW_SIZE2; dy < SAD_WINDOW_SIZE2; dy++) {
		uint8_t disparity = abs(left_image->pixels[x + SAD_WINDOW_SIZE2][y + dy].g
				- right_image->pixels[x + SAD_WINDOW_SIZE2 - d][y + dy].g);

		score_buffer[SAD_WINDOW_SIZE - 1] += disparity;
	}

	// Rather than recompute the whole sum, we know that the current score
	// is the last score, plus the row we just added, minus the row we just
	// removed.
	return score_buffer[SAD_WINDOW_SIZE - 1] + prev_score - last_row_score;
}
