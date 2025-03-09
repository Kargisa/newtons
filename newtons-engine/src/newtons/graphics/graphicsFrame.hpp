#pragma once

#include "newtons/pch.hpp"
#include "graphicsContext.hpp"

namespace nwt
{
	class GraphicsFrame {
	private:
		std::queue<int> _presentQueue;
		bool _recording = false;
		GraphicsContext* _context;

	public:
		GraphicsFrame() = default;
		~GraphicsFrame() = default;

		inline bool isRecording() const { return _recording; }

	private:
		void record();
		void submit();
	};
} // namespace nwt