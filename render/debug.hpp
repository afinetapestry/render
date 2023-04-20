#pragma once

#include "vec3.hpp"

namespace render {
	bool vec3nan(render::vec3 & v) {
		return std::isnan(v.x()) ||
			std::isnan(v.y()) ||
			std::isnan(v.z());
	}
}
