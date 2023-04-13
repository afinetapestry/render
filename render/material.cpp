#include <cstdlib>

#include "material.hpp"

#define random() ((fptype)std::rand() / RAND_MAX)

fptype schlick(fptype cosine, fptype ref);

bool render::lambertian::scatter(const ray & in, const hit_record & rec, vec3 & attenuation, ray & scattered) const {
	auto target = rec.p + rec.normal + random_in_unit_sphere();
	scattered = render::ray(rec.p, target - rec.p);
	attenuation = albedo;
	return true;
}

bool render::metal::scatter(const ray & in, const hit_record & rec, vec3 & attenuation, ray & scattered) const {
	auto reflected = reflect(unit_vector(in.direction()), rec.normal);
	scattered = render::ray(rec.p, reflected + f * random_in_unit_sphere());
	attenuation = albedo;
	return dot(scattered.direction(), rec.normal) > 0.0;
}

bool render::dielectric::scatter(const ray & in, const hit_record & rec, vec3 & attenuation, ray & scattered) const {
	vec3 outward_normal;
	vec3 reflected = reflect(in.direction(), rec.normal);
	float ni_over_nt;
	attenuation = vec3(1.0, 1.0, 1.0);
	vec3 refracted;
	float reflect_prob;
	float cosine;
	if (dot(in.direction(), rec.normal) > 0) {
		outward_normal = -rec.normal;
		ni_over_nt = ref;
		cosine = ref * dot(in.direction(), rec.normal) / in.direction().length();
	}
	else {
		outward_normal = rec.normal;
		ni_over_nt = 1.0 / ref;
		cosine = -dot(in.direction(), rec.normal) / in.direction().length();
	}
	if (refract(in.direction(), outward_normal, ni_over_nt, refracted)) {
		reflect_prob = schlick(cosine, ref);
	}
	else {
		scattered = ray(rec.p, reflected);
		reflect_prob = 1.0;
	}
	if (random() < reflect_prob) {
		scattered = ray(rec.p, reflected);
	}
	else {
		scattered = ray(rec.p, refracted);
	}
	return true;
}

fptype schlick(fptype cosine, fptype ref) {
	fptype r0 = (1.0 - ref) / (1.0 + ref);
	fptype r1 = r0 * r0;
	return r1 + (1.0 * r1) * std::pow(1.0 - cosine, 5.0);
}
