#include "sphere.hpp"

bool render::sphere::hit(const ray & r, fptype t_min, fptype t_max, hit_record & rec) const {
	render::vec3 oc = r.origin() - center;
	fptype a = render::dot(r.direction(), r.direction());
	fptype b = render::dot(oc, r.direction());
	fptype c = dot(oc, oc) - radius * radius;
	fptype discriminant = b * b - a * c;

	if (discriminant > 0.0) {
		fptype temp = (-b - std::sqrt(discriminant)) / a;
		if (temp > t_min && temp < t_max) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		temp = (-b + std::sqrt(discriminant)) / a;
		if (temp > t_min && temp < t_max) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	
	return false;
}
