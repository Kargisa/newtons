#pragma once

#include "event.hpp"

namespace nwt {
class MouseMovedEvent : public Event {
private:
	float _posX, _posY;

public:
	MouseMovedEvent(float posX, float posY)
		: _posX(posX), _posY(posY){}

public:
	constexpr EventType getEventType() override {
		return EventType::MouseMoved;
	}

	float getX() {
		return _posX;
	}

	float getY() {
		return _posY;
	}
	
};

class MousePressedEvent : public Event {
private:
	int _button;

public:
	MousePressedEvent(int button)
		: _button(button) {}

public:
	constexpr EventType getEventType() override {
		return EventType::MousePressed;
	}

	int getButton() const {
		return _button;
	}
};

class MouseReleasedEvent : public Event {
private:
	int _button;

public:
	MouseReleasedEvent(int button)
		: _button(button) {}

public:
	constexpr EventType getEventType() override {
		return EventType::MouseReleased;
	}

	int getButton() const {
		return _button;
	}
};

class MouseScrolledEvent : public MouseEvent {
private:
	float _offsetX, _offsetY;

public:
	MouseScrolledEvent(float offsetX, float offsetY)
		: _offsetX(offsetX), _offsetY(offsetY) {}

public:
	constexpr EventType getEventType() override {
		return EventType::MouseScrolled;
	}

	float getX() const {
		return _offset.x;
	}

	float getY() const {
		return _offset.y;
	}
};

} // namespace nwt