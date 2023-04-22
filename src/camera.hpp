#pragma once

#include "ray.hpp"
#include "vec3.hpp"

namespace render {
	struct camera {
		camera(const vec3 lookfrom, vec3 lookat, vec3 up, const fptype fov, const fptype aspect, const fptype aperture, const fptype depth);

		ray get_ray(const fptype u, const fptype v) const;

		vec3 origin;
		vec3 lower_left_corner;
		vec3 horizontal;
		vec3 vertical;
		vec3 u, v, w;
		fptype lens_radius;
	};
}
