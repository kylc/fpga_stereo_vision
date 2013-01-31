#include <ap_cint.h>

#include "stereobm_config.h"
#include "stereobm_core.h"
#include "stereobm_correspondence.h"
#include "stereobm_image.h"
#include "stereobm_prefilter.h"

struct depth_image_t stereobm_core(struct rgb_image_t *left_image, struct rgb_image_t *right_image) {
	stereobm_prefilter(left_image);
	stereobm_prefilter(right_image);

	struct depth_image_t depth_map = stereobm_correspondence(left_image, right_image);

	return depth_map;
}
