#pragma once

#include "event.hpp"

namespace nwt {
	class NWT_API WindowClosedEvent : public Event {
	public:
		EventType getEventType() const override {
			return EventType::WindowClosed;
		}
	};

	class WindowResizedEvent : public Event {
	private:
		int _width, _height;

	public:
		WindowResizedEvent(int width, int height)
			: _width(width), _height(height) {
		}

		EventType getEventType() const override {
			return EventType::WindowResized;
		}

		int width() const {
			return _width;
		}

		int height() const {
			return _height;
		}

	};
} // namespace nwt