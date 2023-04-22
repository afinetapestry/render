#include "camera.hpp"

render::camera::camera(const vec3 lookfrom, vec3 lookat, vec3 up, const fptype fov, const fptype aspect, const fptype aperture, const fptype depth) {
	lens_radius = aperture / 2.0;
	fptype half_width = std::tan(fov / 2.0);
	fptype half_height = half_width / aspect;
	origin = lookfrom;
	w = unit_vector(lookfrom - lookat);
	u = unit_vector(cross(up, w));
	v = cross(w, u);
	lower_left_corner = origin - half_width * depth * u - half_height * depth * v - depth * w;
	horizontal = 2.0 * half_width * depth * u;
	vertical = 2.0 * half_height * depth * v;
}

render::ray render::camera::get_ray(const fptype s, const fptype t) const {
	vec3 rd = lens_radius * random_in_unit_disk();
	vec3 offset = u * rd.x() + v * rd.y();
	return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
}
