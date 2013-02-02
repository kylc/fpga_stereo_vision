#include "stereobm_core.h"
#include "stereobm_image.h"
#include "tb_util.c"

int main(int argc, char **argv) {
	// Open the test  images
	struct rgb_image_t left_image = load_rgb_ppm("test_images/bowling2_left.ppm");
	struct rgb_image_t right_image = load_rgb_ppm("test_images/bowling2_right.ppm");

	// Run the core
	struct depth_image_t depth = stereobm_core(&left_image, &right_image);

	// Copy data back into PPM structure
        save_depth_ppm("test_images/bowling2_out.ppm", &depth);

	return EXIT_SUCCESS;
}
