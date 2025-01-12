#pragma once

namespace nwt{
class Hash {
public:
	static void HashCombine(size_t& lhs, size_t rhs);
};

inline void Hash::HashCombine(size_t& lhs, size_t rhs) {
	if constexpr (sizeof(size_t) >= 8) {
		lhs ^= rhs + static_cast<size_t>(0x517cc1b727220a95) + (lhs << 6) + (lhs >> 2);
	}
	else {
		lhs ^= rhs + static_cast<size_t>(0x9e3779b9) + (lhs << 6) + (lhs >> 2);
	}
}
}