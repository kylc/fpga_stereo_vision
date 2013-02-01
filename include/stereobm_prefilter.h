#ifndef _STEREOBM_PREFILTER_H_
#define _STEREOBM_PREFILTER_H_

#include "stereobm_image.h"

void stereobm_prefilter(struct rgb_image_t *image);
void stereobm_sobel(struct rgb_image_t *image);

#endif
