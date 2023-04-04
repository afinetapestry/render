#include "vec3.hpp"

inline void render::vec3::make_unit_vector() {
	*this = unit_vector(*this);
}

inline const render::vec3 & render::vec3::operator+=(const render::vec3 o) {
	return *this = *this + o;
}

inline const render::vec3 & render::vec3::operator-=(const render::vec3 o) {
	return *this = *this - o;
}

inline const render::vec3 & render::vec3::operator*=(const render::vec3 o) {
	return *this = *this * o;
}

inline const render::vec3 & render::vec3::operator/=(const render::vec3 o) {
	return *this = *this / o;
}

inline const render::vec3 & render::vec3::operator*=(const fptype s) {
	return *this = *this * s;
}

inline const render::vec3 & render::vec3::operator/=(const fptype s) {
	return *this = *this / s;
}
