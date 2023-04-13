#define _USE_MATH_DEFINES

#include <cstdio>
#include <cstdlib>
#include <future>
#include <memory>
#include <vector>

#include "camera.hpp"
#include "fptype.h"
#include "hitable_list.hpp"
#include "material.hpp"
#include "pfm.hpp"
#include "sphere.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#define random() ((fptype)std::rand() / RAND_MAX)
#define WIDTH 640
#define HEIGHT 480
#define SAMPLES 100
#define BOUNCES 50
#define THREADS 8

void render_scanlines(unsigned int y, unsigned int count, unsigned int width, unsigned int height, render::camera & cam, unsigned int samples, fptype * data, const render::hitable & world);
render::vec3 color(const render::ray & r, const render::hitable & world, const int depth);
void writevec3(fptype * data, unsigned width, unsigned x, unsigned y, render::vec3 v);
void writergb(fptype * data, unsigned width, unsigned x, unsigned y, fptype r, fptype g, fptype b);

int main(int argc, const char *argv[]) {
	fptype * data = (fptype *)malloc(WIDTH * HEIGHT * sizeof(render::vec3));

	render::camera cam(render::vec3(-2.0, 2.0, 1.0), render::vec3(0.0, 0.0, -1.0), render::vec3(0.0, 1.0, 0.0), M_PI / 5.0, WIDTH / (fptype)HEIGHT);

	auto a = std::make_shared<render::sphere>(render::vec3(0.0, 0.0, -1.0), 0.5, std::make_shared<render::lambertian>(render::vec3(0.1, 0.2, 0.5)));
	auto b = std::make_shared<render::sphere>(render::vec3(0.0, -100.5, -1.0), 100.0, std::make_shared<render::lambertian>(render::vec3(0.8, 0.8, 0.0)));
	auto c = std::make_shared<render::sphere>(render::vec3(1.0, 0.0, -1.0), 0.5, std::make_shared<render::metal>(render::vec3(0.8, 0.6, 0.2), 0.1));
	auto d = std::make_shared<render::sphere>(render::vec3(-1.0, 0.0, -1.0), 0.5, std::make_shared<render::dielectric>(1.5));
	auto e = std::make_shared<render::sphere>(render::vec3(-1.0, 0.0, -1.0), -0.45, std::make_shared<render::dielectric>(1.5));

	render::hitable_list world;
	world.add(a);
	world.add(b);
	world.add(c);
	world.add(d);
	world.add(e);

	std::vector<std::future<void>> futures;

	unsigned stride = HEIGHT / THREADS;
	for (unsigned y = 0; y < HEIGHT; y += stride) {
		auto f = std::async(std::launch::async, [&, y]() {
			render_scanlines(y, stride, WIDTH, HEIGHT, cam, SAMPLES, data, world);
		});
		futures.push_back(std::move(f));
	}

	for (auto & f : futures) {
		f.wait();
	}

	writepfm(argv[1], WIDTH, HEIGHT, 3, data);
}

void render_scanlines(unsigned int y, unsigned int count, unsigned int width, unsigned int height, render::camera & cam, unsigned int samples, fptype * data, const render::hitable & world) {
	for (unsigned j = y; j < y + count && j < height; ++j) {
		for (unsigned i = 0; i < width; ++i) {
			render::vec3 col;

			for (unsigned n = 0; n < samples; ++n) {
				fptype u = (i + random()) / (fptype)width;
				fptype v = (j + random()) / (fptype)height;

				auto r = cam.get_ray(u, v);
				col += color(r, world, BOUNCES);
			}

			writevec3(data, width, i, j, col / samples);
		}
	}
}

render::vec3 color(const render::ray & r, const render::hitable & world, const int depth) {
	render::hit_record rec;
	if (world.hit(r, 0.001, FLT_MAX, rec)) {
		render::ray scattered;
		render::vec3 attenuation;
		if (depth > 0 && rec.mat->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth - 1);
		}
		else {
			return render::vec3(0.0, 0.0, 0.0);
		}
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
