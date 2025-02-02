#pragma once

#include "event.hpp"

namespace nwt {
class WindowClosedEvent : public Event {
public:
	constexpr EventType getEventType() override {
		return EventType::WindowClosed;
	}
};

class WindowResizedEvent : public Event {
private:
	int _width, _height;

public:
	WindowResizedEvent(int width, int height)
		: _width(width), _height(height) {}

public:
	constexpr EventType getEventType() override {
		return EventType::WindowResized;
	}

	int getWidth() {
		return _width;
	}

	int getHeight() {
		return _height;
	}

};

} // namespace nwt