#pragma once

#include "vec3.hpp"

namespace render {
	struct ray {
		ray() : o{}, d{} {}
		ray(const vec3 & o, const vec3 & d) : o{ o }, d{ d } {}

		vec3 origin() const { return o; }
		vec3 direction() const { return d; }
		vec3 point_at_parameter(fptype p) const { return o + p * d; }

		vec3 o;
		vec3 d;
	};
}
