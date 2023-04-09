#pragma once

#include "fptype.h"
#include "vec3.hpp"
#include "ray.hpp"

namespace render {
	struct hit_record {
		fptype t;
		vec3 p;
		vec3 normal;
	};

	class hitable {
	public:
		virtual bool hit(const ray & r, fptype t_min, fptype t_max, hit_record & h) const = 0;
	};
}
