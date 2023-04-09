#include "vec3.hpp"

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
