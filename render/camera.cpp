#include "camera.hpp"

#include <cstdlib>
#include <ctime>

render::camera::camera(const unsigned int width, const unsigned int height) {
	std::srand(std::time(nullptr));

	const fptype aspect = width / (fptype)height;

	lower_left_corner = render::vec3(-1.0, -1.0 / aspect, -1.0);
	horizontal = render::vec3(2.0, 0.0, 0.0);
	vertical = render::vec3(0.0, 2.0 / aspect, 0.0);
	origin = render::vec3(0.0, 0.0, 0.0);
}

render::ray render::camera::get_ray(const fptype u, const fptype v) const {
	return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
}
