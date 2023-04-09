#include <cstdio>
#include <memory>

#include "camera.hpp"
#include "fptype.h"
#include "hitable_list.hpp"
#include "pfm.hpp"
#include "sphere.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#define rand() ((fptype)std::rand() / RAND_MAX)

render::vec3 color(const render::ray & r, const render::hitable & world);
void writevec3(fptype * data, unsigned width, unsigned x, unsigned y, render::vec3 v);
void writergb(fptype * data, unsigned width, unsigned x, unsigned y, fptype r, fptype g, fptype b);

int main(int argc, const char *argv[]) {
	const unsigned width = 640, height = 480, samples = 100;

	fptype * data = (fptype *)malloc(width * height * sizeof(render::vec3));

	render::camera cam(width, height);

	auto a = std::make_shared<render::sphere>(render::vec3(0.0, 0.0, -1.0), 0.5);
	auto b = std::make_shared<render::sphere>(render::vec3(0.0, -100.5, -1.0), 100.0);

	render::hitable_list world;
	world.add(a);
	world.add(b);

	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			render::vec3 col;

			for (unsigned n = 0; n < samples; ++n) {
				fptype u = (x + rand()) / (fptype)width;
				fptype v = (y + rand()) / (fptype)height;

				auto r = cam.get_ray(u, v);
				col += color(r, world);
			}

			writevec3(data, width, x, y, col / samples);
		}

		std::printf(".");
	}

	writepfm(argv[1], width, height, 3, data);
}

render::vec3 color(const render::ray & r, const render::hitable & world) {
	render::hit_record rec;
	if (world.hit(r, 0.0, FLT_MAX, rec)) {
		return 0.5 * (render::vec3(rec.normal.x() + 1.0, rec.normal.y() + 1.0, rec.normal.z() + 1.0));
	}
	else {
		render::vec3 unit_direction = render::unit_vector(r.direction());
		fptype t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * render::vec3(1.0, 1.0, 1.0) + t * render::vec3(0.5, 0.7, 1.0);
	}
}

void writevec3(fptype * data, unsigned width, unsigned x, unsigned y, render::vec3 v) {
	writergb(data, width, x, y, v.r(), v.g(), v.b());
}

void writergb(fptype * data, unsigned width, unsigned x, unsigned y, fptype r, fptype g, fptype b) {
	data[(y * width + x) * 3 + 0] = r;
	data[(y * width + x) * 3 + 1] = g;
	data[(y * width + x) * 3 + 2] = b;
}
