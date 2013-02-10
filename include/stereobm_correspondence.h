#ifndef _STEREOBM_CORRESPONDENCE_H_
#define _STEREOBM_CORRESPONDENCE_H_

#include "stereobm_image.h"

struct depth_image_t stereobm_correspondence(struct g_image_t *left_image, struct g_image_t *right_image);
int32_t stereobm_sad_score(struct g_image_t *left_image, struct g_image_t *right_image, uint16_t x, uint16_t y, uint8_t d, int32_t score_buffer[SAD_WINDOW_SIZE]);
int32_t stereobm_progressive_sad_score(struct g_image_t *left_image, struct g_image_t *right_image, uint16_t x, uint16_t y, uint8_t d, int32_t score_buffer[SAD_WINDOW_SIZE]);

#endif
