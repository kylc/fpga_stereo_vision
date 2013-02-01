#ifndef _STEREOBM_CORRESPONDENCE_H_
#define _STEREOBM_CORRESPONDENCE_H_

#include "stereobm_image.h"

struct depth_image_t stereobm_correspondence(struct rgb_image_t *left_image, struct rgb_image_t *right_image);

#endif
