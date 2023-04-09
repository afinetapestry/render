#pragma once

#include <list>
#include <memory>

#include "hitable.hpp"

namespace render {
	class hitable_list : public hitable {
		std::list<std::shared_ptr<hitable>> list;

	public:
		hitable_list() : list{} {}
		
		void add(std::shared_ptr<hitable> h) {
			list.push_back(h);
		}

		bool hit(const ray & r, fptype t_min, fptype t_max, hit_record & rec) const override;
	};
}

bool render::hitable_list::hit(const ray & r, fptype t_min, fptype t_max, hit_record & rec) const {
	hit_record temp;
	bool hit = false;
	fptype closest = t_max;
	for (auto const & i : list) {
		if (i->hit(r, t_min, closest, temp)) {
			hit = true;
			closest = temp.t;
			rec = temp;
		}
	}
	return hit;
}
