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
		static float sqrt(float value);
		static constexpr float lerp(float a, float b, float t);
		static constexpr float min(float a, float b);
		static constexpr float max(float a, float b);
		static constexpr float abs(float value);
		static float pow(float a, float n);
		static float clamp(float x, float min, float max);
		static float clamp01(float x);
		static float sin(float x);
		static float cos(float x);
		static float asin(float x);
		static float acos(float x);
	};

	inline float Mathf::sqrt(float value) {
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

	inline float Mathf::pow(float a, float n){
		return std::powf(a, n);
	}

	inline float Mathf::clamp(float x, float min, float max){
		return x > max ? max : (x < min ? min : x);
	}

	inline float Mathf::clamp01(float x){
		return x > 1 ? 1 : (x < 0 ? 0 : x);
	}

	inline float Mathf::sin(float x){
		return std::sinf(x);
	}

	inline float Mathf::cos(float x){
		return std::cosf(x);
	}

	inline float Mathf::asin(float x){
		return std::asinf(x);
	}

	inline float Mathf::acos(float x){
		return std::acosf(x);
	}


} // namespace nwt