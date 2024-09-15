#pragma once

#include <iostream>
#include <cmath>


namespace Newtons {
	class Mathf {
	public:
		Mathf() = delete;
		static float PI();
		static float Sqrt(float value);
		static float Lerp(float a, float b, float t);
		static float Min(float a, float b);
		static float Max(float a, float b);
		static float Abs(float value);
	};

	inline float Mathf::PI() {
		return 3.1415927f;
	}

	inline float Mathf::Sqrt(float value) {
		return std::sqrt(value);
	}

	inline float Mathf::Lerp(float a, float b, float t) {
		return a + (b - a) * t;
	}

	inline float Mathf::Min(float a, float b) {
		return std::fmin(a, b);
	}

	inline float Mathf::Max(float a, float b) {
		return std::fmax(a, b);
	}

	inline float Mathf::Abs(float value) {
		return std::abs(value);
	}
} // namespace Newtons