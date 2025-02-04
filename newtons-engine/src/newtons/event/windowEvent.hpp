#pragma once

#include "event.hpp"

namespace nwt {
class NWT_API WindowClosedEvent : public Event {
public:
	EventType getEventType() const override {
		return EventType::WindowClosed;
	}
};

class NWT_API WindowResizedEvent : public Event {
private:
	int _width, _height;

public:
	WindowResizedEvent(int width, int height)
		: _width(width), _height(height) {}

public:
	EventType getEventType() const override {
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