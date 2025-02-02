#pragma once

#include "event.hpp"

namespace nwt {
class KeyEvent : public Event {
public:
	int _keycode;

public:
	KeyEvent() = delete;
protected:
	KeyEvent(int keycode)
		: _keycode(keycode) {}
public:
	int getKeycode() const { return _keycode }

};

class KeyPressedEvent : public KeyEvent {
public:
	KeyPressedEvent() = delete;
	KeyPressedEvent(int keycode)
		: KeyEvent(keycode) {}

	constexpr EventType getEventType() override {
		return EventType::KeyPressed;
	}
};

class KeyReleasedEvent : public KeyEvent {
public:
	KeyReleasedEvent() = delete;
	KeyReleasedEvent(int keycode)
		: KeyEvent(keycode) {}

	constexpr EventType getEventType() override {
		return EventType::KeyReleased;
	}
};

} // namespace nwt