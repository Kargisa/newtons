#pragma once

#include "newtons/pch.hpp"

namespace nwt
{
	class GraphicsFrame {
	private:
		std::queue<int> _presentQueue;
		bool _recording = false;

	public:
		GraphicsFrame() = default;
		~GraphicsFrame() = default;

		void record();
		void submit();
		inline bool isRecording() const { return _recording; }
	};
} // namespace nwt