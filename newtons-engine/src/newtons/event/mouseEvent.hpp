#pragma once

#include "event.hpp"

namespace nwt {
class NWT_API MouseMovedEvent : public Event {
private:
	float _posX, _posY;

public:
	MouseMovedEvent(float posX, float posY)
		: _posX(posX), _posY(posY) {}

public:
	EventType getEventType() const override {
		return EventType::MouseMoved;
	}

	float getX() {
		return _posX;
	}

	float getY() {
		return _posY;
	}
	
};

class NWT_API MousePressedEvent : public Event {
private:
	int _button;

public:
	MousePressedEvent(int button)
		: _button(button) {}

public:
	EventType getEventType() const override {
		return EventType::MousePressed;
	}

	int getButton() const {
		return _button;
	}
};

class NWT_API MouseReleasedEvent : public Event {
private:
	int _button;

public:
	MouseReleasedEvent(int button)
		: _button(button) {}

public:
	EventType getEventType() const override {
		return EventType::MouseReleased;
	}

	int getButton() const {
		return _button;
	}
};

class NWT_API MouseScrolledEvent : public Event {
private:
	float _offsetX, _offsetY;

public:
	MouseScrolledEvent(float offsetX, float offsetY)
		: _offsetX(offsetX), _offsetY(offsetY) {}

public:
	EventType getEventType() const override {
		return EventType::MouseScrolled;
	}

	float getX() const {
		return _offsetX;
	}

	float getY() const {
		return _offsetY;
	}
};

} // namespace nwt