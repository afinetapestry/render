#include <iostream>

#include "fptype.h"
#include "pfm.hpp"
#include "vec3.hpp"

void writevec3(fptype * data, unsigned width, unsigned x, unsigned y, render::vec3 v);
void writergb(fptype * data, unsigned width, unsigned x, unsigned y, fptype r, fptype g, fptype b);

int main(int argc, const char *argv[]) {
	const unsigned width = 640, height = 480;

	fptype * data = (fptype *)malloc(width * height * sizeof(render::vec3));

	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			//writevec3(data, width, x, y, render::vec3(x / (fptype)width, y / (fptype)height, 0.2));
		}
	}

	writepfm(argv[1], width, height, 3, data);
}

void writevec3(fptype * data, unsigned width, unsigned x, unsigned y, render::vec3 v) {
	writergb(data, width, x, y, v.r(), v.g(), v.b());
}

void writergb(fptype * data, unsigned width, unsigned x, unsigned y, fptype r, fptype g, fptype b) {
	data[(y * width + x) * 3 + 0] = r;
	data[(y * width + x) * 3 + 1] = g;
	data[(y * width + x) * 3 + 2] = b;
}
