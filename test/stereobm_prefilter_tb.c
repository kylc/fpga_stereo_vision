#include <stdlib.h>
#include <time.h>

#include "stereobm_image.h"
#include "stereobm_prefilter.h"

int main() {
	// Seed the random number generator
	srand(time(NULL));

	// Generate a test image
	struct rgb_image_t image;
	for(int x = 0; x < IMG_WIDTH; x++) {
		for(int y = 0; y < IMG_HEIGHT; y++) {
		    image.pixels[x][y].r = rand() % 255;
		    image.pixels[x][y].g = rand() % 255;
		    image.pixels[x][y].b = rand() % 255;
		}
	}

	// Apply the prefilter
	stereobm_prefilter(&image);

	return 0;
}
