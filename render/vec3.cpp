#define _USE_MATH_DEFINES

#include <cstdlib>

#include "vec3.hpp"

#define random() ((fptype)std::rand() / RAND_MAX)

void render::vec3::make_unit_vector() {
	*this = unit_vector(*this);
}

const render::vec3 & render::vec3::operator+=(const render::vec3 o) {
	return *this = *this + o;
}

const render::vec3 & render::vec3::operator-=(const render::vec3 o) {
	return *this = *this - o;
}

const render::vec3 & render::vec3::operator*=(const render::vec3 o) {
	return *this = *this * o;
}

const render::vec3 & render::vec3::operator/=(const render::vec3 o) {
	return *this = *this / o;
}

const render::vec3 & render::vec3::operator*=(const fptype s) {
	return *this = *this * s;
}

const render::vec3 & render::vec3::operator/=(const fptype s) {
	return *this = *this / s;
}

render::vec3 render::random_in_unit_sphere() {
	fptype u = random();
	fptype v = random();
	fptype theta = u * 2.0 * M_PI;
	fptype phi = std::acos(2.0 * v - 1.0);
	fptype r = std::cbrt(random());
	fptype sinTheta = std::sin(theta);
	fptype cosTheta = std::cos(theta);
	fptype sinPhi = std::sin(phi);
	fptype cosPhi = std::cos(phi);
	fptype x = r * sinPhi * cosTheta;
	fptype y = r * sinPhi * sinTheta;
	fptype z = r * cosPhi;
	return render::vec3(x, y, z);
}

render::vec3 render::reflect(const render::vec3 & v, const render::vec3 & n) {
	return v - 2 * dot(v, n) * n;
}

bool render::refract(const vec3 & v, const vec3 & n, const float ni_over_nt, vec3 & refracted) {
	vec3 uv = unit_vector(v);
	fptype dt = dot(uv, n);
	fptype discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
	if (discriminant > 0.0) {
		refracted = ni_over_nt * (uv - n * dt) - n * std::sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
}
