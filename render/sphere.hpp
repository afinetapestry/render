#pragma once

#include "hitable.hpp"

namespace render {
	class sphere : public hitable {
		vec3 center;
		fptype radius;
		std::shared_ptr<material> mat;

	public:
		sphere(vec3 c, fptype r, std::shared_ptr<material> mat) : center { c }, radius{ r }, mat{ mat } {}

		bool hit(const ray & r, fptype t_min, fptype t_max, hit_record & rec) const override;
	};
}
