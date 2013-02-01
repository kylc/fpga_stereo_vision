#ifndef _STEREOBM_CORE_H_
#define _STEREOBM_CORE_H_

#include "stereobm_image.h"

struct depth_image_t stereobm_core(struct rgb_image_t *left_image, struct rgb_image_t *right_image);

#endif
