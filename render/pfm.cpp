#include <cstdio>
#include <iostream>

#include "pfm.hpp"

bool writepfm(const char * path, unsigned width, unsigned height, unsigned channels, fptype * data) {
	FILE * f;

	if (!(f = fopen(path, "wb")))
	{
		std::cerr << "Could not open file: " << path << "\n";
		return false;
	}

	fprintf(f, "PF\n%d %d\n-1.0\n", width, height);
	fwrite(data, sizeof(float), width * height * channels, f);
	fclose(f);

	return true;
}
