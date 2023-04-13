#include "camera.hpp"

#include <cstdlib>
#include <ctime>

render::camera::camera(const vec3 lookfrom, vec3 lookat, vec3 up, const fptype fov, const fptype aspect) {
	std::srand(std::time(nullptr));

	fptype half_width = std::tan(fov / 2.0);
	fptype half_height = half_width / aspect;
	origin = lookfrom;
	vec3 w = unit_vector(lookfrom - lookat);
	vec3 u = unit_vector(cross(up, w));
	vec3 v = cross(w, u);

	vec3 plane = vec3(-half_width, -half_height, -1.0);
	lower_left_corner = origin - half_width * u - half_height * v - w;
	horizontal = 2.0 * half_width * u;
	vertical = 2.0 * half_height * v;
}

render::ray render::camera::get_ray(const fptype u, const fptype v) const {
	return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
}
