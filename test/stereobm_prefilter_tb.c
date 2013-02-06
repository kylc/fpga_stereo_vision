#include "stereobm_image.h"
#include "stereobm_prefilter.h"
#include "tb_util.c"

int main(int argc, char **argv) {
	// Open the test test image
	struct rgb_image_t image = load_rgb_ppm("test_images/im2.ppm");

	// Apply the prefilter
	struct g_image_t prefiltered = stereobm_prefilter(&image);

	// Copy data back into PPM structure
	save_grayscale_ppm("test_images/im2_out.ppm", &prefiltered, SOBEL_CLAMP);

	return EXIT_SUCCESS;
}
