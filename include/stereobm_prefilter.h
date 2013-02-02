#ifndef _STEREOBM_PREFILTER_H_
#define _STEREOBM_PREFILTER_H_

#include "stereobm_image.h"

struct g_image_t stereobm_prefilter(struct rgb_image_t *image);
struct g_image_t stereobm_convert_to_grayscale(struct rgb_image_t *image);
struct g_image_t stereobm_sobel(struct g_image_t *image);

#endif
