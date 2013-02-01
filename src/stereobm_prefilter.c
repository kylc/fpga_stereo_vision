#include "stereobm_config.h"
#include "stereobm_image.h"
#include "stereobm_prefilter.h"

void stereobm_prefilter(struct rgb_image_t *image) {
	stereobm_sobel(image);
}

/* Applies a Sobel filter to the image */
void stereobm_sobel(struct rgb_image_t *image) {

}
