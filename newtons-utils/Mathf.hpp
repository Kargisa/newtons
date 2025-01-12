#pragma once

#include <cmath>


namespace nwt {
	class Mathf {
	public:
		Mathf() = delete;
		static constexpr float PI = 3.1415927f;
		static constexpr float RadToDeg = 57.29578f;
		static constexpr float DegToRad = 0.017453292f;
	public:
		static constexpr float sqrt(float value);
		static constexpr float lerp(float a, float b, float t);
		static constexpr float min(float a, float b);
		static constexpr float max(float a, float b);
		static constexpr float abs(float value);
		static constexpr float pow(float a, float n);
	};

	inline constexpr float Mathf::sqrt(float value) {
		return std::sqrt(value);
	}

	inline constexpr float Mathf::lerp(float a, float b, float t) {
		return a + (b - a) * t;
	}

	inline constexpr float Mathf::min(float a, float b) {
		return a < b ? a : b;
	}

	inline constexpr float Mathf::max(float a, float b) {
		return a > b ? a : b;
	}

	inline constexpr float Mathf::abs(float value) {
		return value < 0 ? -value : value;
	}

	inline constexpr float Mathf::pow(float a, float n){
		return std::powf(a, n);
	}
} // namespace nwt