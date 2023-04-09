#pragma once

#include "hitable.hpp"

namespace render {
	class sphere : public hitable {
		vec3 center;
		fptype radius;

	public:
		sphere() : center{ 0.0, 0.0, 0.0 }, radius{ 1.0 } {}
		sphere(vec3 c, fptype r) : center{ c }, radius{ r } {}

		bool hit(const ray & r, fptype t_min, fptype t_max, hit_record & rec) const override;
	};
}
