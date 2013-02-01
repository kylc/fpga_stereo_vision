#include <stdlib.h>
#include <pam.h>

#include "stereobm_image.h"
#include "stereobm_prefilter.h"

int main(int argc, char **argv) {
	// Open the test test image
	pm_init(argv[0], 0);

	int cols, rows;
	pixval max;
	FILE *fp = fopen("test_img.ppm", "r");
	pixel **data = ppm_readppm(fp, &cols, &rows, &max);

	// Copy data into internal data structure
	struct rgb_image_t image;
	for(int x = 0; x < IMG_WIDTH; x++) {
		for(int y = 0; y < IMG_HEIGHT; y++) {
			// PPM data is indexed by y, then x
			image.pixels[x][y].r = PPM_GETR(data[y][x]);
			image.pixels[x][y].g = PPM_GETG(data[y][x]);
			image.pixels[x][y].b = PPM_GETB(data[y][x]);
		}
	}

	// Apply the prefilter
	stereobm_prefilter(&image);

	// Copy data back into PPM structure
	for(int x = 0; x < IMG_WIDTH; x++) {
		for(int y = 0; y < IMG_HEIGHT; y++) {
			// PPM data is indexed by y, then x
			struct rgb8_t rgb = image.pixels[x][y];
			PPM_ASSIGN(data[y][x], rgb.r, rgb.g, rgb.b);
		}
	}

	FILE *outfp = fopen("test_img_out.ppm", "w");
	ppm_writeppm(outfp, data, cols, rows, max, 0);

	return EXIT_SUCCESS;
}
