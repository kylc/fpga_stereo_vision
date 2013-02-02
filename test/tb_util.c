#include <pam.h>

#include "stereobm_image.h"

struct rgb_image_t load_rgb_ppm(char *file_name);
void save_grayscale_ppm(char *file_name, struct g_image_t *image);
void save_depth_ppm(char *file_name, struct depth_image_t *image);

struct rgb_image_t load_rgb_ppm(char *file_name) {
	int cols, rows;
	pixval max;
	FILE *fp = fopen(file_name, "r");
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

	return image;
}

void save_grayscale_ppm(char *file_name, struct g_image_t *image) {
	pixel **data = ppm_allocarray(IMG_WIDTH, IMG_HEIGHT);

	// Copy data back into PPM structure
	for(int x = 0; x < IMG_WIDTH; x++) {
		for(int y = 0; y < IMG_HEIGHT; y++) {
			struct g8_t pixel = image->pixels[x][y];

			// PPM data is indexed by y, then x
			PPM_ASSIGN(data[y][x], pixel.g, pixel.g, pixel.g);
		}
	}

	FILE *outfp = fopen(file_name, "w");
	ppm_writeppm(outfp, data, IMG_WIDTH, IMG_HEIGHT, PPM_MAXMAXVAL, 0);
}

void save_depth_ppm(char *file_name, struct depth_image_t *image) {
	pixel **data = ppm_allocarray(IMG_WIDTH, IMG_HEIGHT);

	// Copy data back into PPM structure
	for(int x = 0; x < IMG_WIDTH; x++) {
		for(int y = 0; y < IMG_HEIGHT; y++) {
			struct depth8_t p = image->pixels[x][y];

			// PPM data is indexed by y, then x
			PPM_ASSIGN(data[y][x], p.d, p.d, p.d);
		}
	}

	FILE *outfp = fopen(file_name, "w");
	ppm_writeppm(outfp, data, IMG_WIDTH, IMG_HEIGHT, PPM_MAXMAXVAL, 0);
}
