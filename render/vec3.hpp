#pragma once

#include <cmath>

#include "fptype.h"

namespace render {
	struct vec3 {
		vec3() : e{} {}
		vec3(fptype e0, fptype e1, fptype e2) : e{ e0, e1, e2 } {}

		inline fptype x() { return e[0]; }
		inline fptype y() { return e[1]; }
		inline fptype z() { return e[2]; }
		inline fptype r() { return e[0]; }
		inline fptype g() { return e[1]; }
		inline fptype b() { return e[2]; }

		inline const vec3 & operator+() const { return *this; }
		inline const vec3 & operator-() const { return vec3(-e[0], -e[1], -e[2]); }
		inline const fptype operator[](int i) const { return e[i]; }
		inline const fptype& operator[](int i) { return e[i]; }

		const vec3 & operator+=(const vec3 o);
		const vec3 & operator-=(const vec3 o);
		const vec3 & operator*=(const vec3 o);
		const vec3 & operator/=(const vec3 o);
		const vec3 & operator*=(const fptype s);
		const vec3 & operator/=(const fptype s);

		inline fptype length() const {
			return std::sqrt(squared_length());
		}

		inline fptype squared_length() const {
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		}

		void make_unit_vector();

		fptype e[3];
	};

	inline vec3 operator+(const vec3 & v0, const vec3 & v1) {
		return vec3(v0.e[0] + v1.e[0], v0.e[1] + v1.e[1], v0.e[2] + v1.e[2]);
	}

	inline vec3 operator-(const vec3 & v0, const vec3 & v1) {
		return vec3(v0.e[0] - v1.e[0], v0.e[1] - v1.e[1], v0.e[2] - v1.e[2]);
	}

	inline vec3 operator*(const vec3 & v0, const vec3 & v1) {
		return vec3(v0.e[0] * v1.e[0], v0.e[1] * v1.e[1], v0.e[2] * v1.e[2]);
	}

	inline vec3 operator/(const vec3 & v0, const vec3 & v1) {
		return vec3(v0.e[0] / v1.e[0], v0.e[1] / v1.e[1], v0.e[2] / v1.e[2]);
	}

	inline vec3 operator*(const vec3 & v, const fptype s) {
		return vec3(v.e[0] * s, v.e[1] * s, v.e[2] * s);
	}

	inline vec3 operator*(const fptype s, const vec3 & v) {
		return v * s;
	}

	inline vec3 operator/(const vec3 & v, const fptype s) {
		return vec3(v.e[0] / s, v.e[1] / s, v.e[2] / s);
	}

	inline fptype dot(const vec3 & v0, const vec3 & v1) {
		return v0.e[0] * v1.e[0] + v0.e[1] * v1.e[1] + v0.e[2] * v1.e[2];
	}

	inline vec3 cross(const vec3 & v0, const vec3 & v1) {
		return vec3(v0.e[1] * v1.e[2] - v0.e[2] * v1.e[1],
			-(v0.e[0] * v1.e[2] - v0.e[2] * v1.e[0]),
			v0.e[0] * v1.e[1] - v0.e[1] * v1.e[0]
		);
	}

	inline vec3 unit_vector(const vec3 & v) {
		return v / v.length();
	}
}
