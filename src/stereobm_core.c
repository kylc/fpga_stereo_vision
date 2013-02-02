#include "stereobm_config.h"
#include "stereobm_core.h"
#include "stereobm_correspondence.h"
#include "stereobm_image.h"
#include "stereobm_prefilter.h"

struct depth_image_t stereobm_core(struct rgb_image_t *left_image, struct rgb_image_t *right_image) {
	// Prefilter both input images
	struct g_image_t gray_left = stereobm_prefilter(left_image);
	struct g_image_t gray_right = stereobm_prefilter(right_image);

	// Create the correspondence map
	struct depth_image_t depth_map = stereobm_correspondence(&gray_left, &gray_right);

	return depth_map;
}
