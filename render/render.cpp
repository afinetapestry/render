#define _USE_MATH_DEFINES

#include <chrono>
#include <iostream>
#include <future>
#include <memory>
#include <vector>

#include "camera.hpp"
#include "chrono.hpp"
#include "fptype.h"
#include "hitable_list.hpp"
#include "material.hpp"
#include "pfm.hpp"
#include "sphere.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#define WIDTH 1920
#define HEIGHT 1080
#define SAMPLES 200
#define BOUNCES 50
#define THREADS 11

render::hitable_list random_scene();
void render_scanlines(unsigned int y, unsigned int count, unsigned int width, unsigned int height, render::camera & cam, unsigned int samples, fptype * data, const render::hitable & world);
render::vec3 color(const render::ray & r, const render::hitable & world, const int depth);
void writevec3(fptype * data, unsigned width, unsigned x, unsigned y, render::vec3 v);
void writergb(fptype * data, unsigned width, unsigned x, unsigned y, fptype r, fptype g, fptype b);

int main(int argc, const char *argv[]) {
	fptype * data = (fptype *)malloc(WIDTH * HEIGHT * sizeof(render::vec3));

	auto lookfrom = render::vec3(13.0, 2.0, 3.0);
	auto lookat = render::vec3(0.0, 0.0, 0.0);
	auto focus = 10.0;
	render::camera cam(lookfrom, lookat, render::vec3(0.0, 1.0, 0.0), M_PI / 5.0, WIDTH / (fptype)HEIGHT, 0.1, focus);

	auto world = random_scene();

	auto start = std::chrono::system_clock::now();

#if THREADS > 1
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
#else
	render_scanlines(0, HEIGHT, WIDTH, HEIGHT, cam, SAMPLES, data, world);
#endif

	auto end = std::chrono::system_clock::now();

	auto elapsed = end - start;

	std::cout << "Rendered in: " << elapsed << "\n";

	writepfm(argv[1], WIDTH, HEIGHT, 3, data);
}

render::hitable_list random_scene() {
	render::hitable_list world;
	world.add(std::make_shared<render::sphere>(render::vec3(0.0, -1000.0, 0.0), 1000.0, std::make_shared<render::lambertian>(render::vec3(0.5, 0.5, 0.5))));

	for (int x = -11; x <= 11; ++x) {
		for (int y = -11; y <= 11; ++y) {
			render::vec3 center(x + 0.7 * random(), 0.2, y + 0.7 * random());

			if ((center - render::vec3(4.0, 0.2, 0.0)).length() <= 0.9) {
				continue;
			}

			float mat = random();
			if (mat < 0.8) {
				world.add(std::make_shared<render::sphere>(center, 0.2, std::make_shared<render::lambertian>(render::vec3(random() * random(), random() * random(), random() * random()))));
			}
			else if (mat < 0.95) {
				world.add(std::make_shared<render::sphere>(center, 0.2, std::make_shared<render::metal>(render::vec3(0.5 * (1.0 + random()), 0.5 * (1.0 + random()), 0.5 * (1.0 + random())), 0.5 * random())));
			}
			else {
				world.add(std::make_shared<render::sphere>(center, 0.2, std::make_shared<render::dielectric>(1.5)));
			}
		}
	}

	world.add(std::make_shared<render::sphere>(render::vec3(0.0, 1.0, 0.0), 1.0, std::make_shared<render::dielectric>(1.5)));
	world.add(std::make_shared<render::sphere>(render::vec3(-4.0, 1.0, 0.0), 1.0, std::make_shared<render::lambertian>(render::vec3(0.4, 0.2, 0.1))));
	world.add(std::make_shared<render::sphere>(render::vec3(4.0, 1.0, 0.0), 1.0, std::make_shared<render::metal>(render::vec3(0.7, 0.6, 0.5), 0.0)));

	return world;
}

void render_scanlines(unsigned int y, unsigned int count, unsigned int width, unsigned int height, render::camera & cam, unsigned int samples, fptype * data, const render::hitable & world) {
	for (unsigned j = y; j < y + count && j < height; ++j) {
		for (unsigned i = 0; i < width; ++i) {
			render::vec3 col;

			for (unsigned n = 0; n < samples; ++n) {
				fptype u = (i + random()) / (fptype)width;
				fptype v = (j + random()) / (fptype)height;

				auto r = cam.get_ray(u, v);
				auto c = color(r, world, BOUNCES);
				col += c;
			}

			writevec3(data, width, i, j, col / samples);
		}
	}

	std::printf("Done: %d-%d\n", y, std::min(y + count, height) - 1);
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
