#include "graphicsFrame.hpp"

namespace nwt
{
	void GraphicsFrame::record() {
		_recording = true;
	}

	void GraphicsFrame::submit() {
		_recording = false;
	}
} // namespace nwt