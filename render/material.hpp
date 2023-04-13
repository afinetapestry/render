#pragma once

#include "hitable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace render {
	class material {
	public:
		virtual bool scatter(const ray & in, const hit_record & rec, vec3 & attenuation, ray & scattered) const = 0;
	};

	class lambertian : public material {
		vec3 albedo;

	public:
		lambertian(const vec3 & a) : albedo(a) {}

		bool scatter(const ray & in, const hit_record & rec, vec3 & attenuation, ray & scattered) const override;
	};

	class metal : public material {
		vec3 albedo;
		fptype f;

	public:
		metal(const vec3 & a, const fptype fuzz = 0.0) : albedo(a), f{ fuzz } {}

		bool scatter(const ray & in, const hit_record & rec, vec3 & attenuation, ray & scattered) const override;
	};

	class dielectric : public material {
		fptype ref;

	public:
		dielectric(const fptype ref_index) : ref{ ref_index } {}

		bool scatter(const ray & in, const hit_record & rec, vec3 & attenuation, ray & scattered) const override;
	};
}
