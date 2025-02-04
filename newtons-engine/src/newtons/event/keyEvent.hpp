#pragma once

#include "event.hpp"

namespace nwt {
class NWT_API KeyEvent : public Event {
public:
	int _keycode;

protected:
	KeyEvent() {}
	KeyEvent(int keycode)
		: _keycode(keycode) {}
public:
	int getKeycode() const { return _keycode; }

};

class NWT_API KeyPressedEvent : public KeyEvent {
public:
	KeyPressedEvent() = delete;
	KeyPressedEvent(int keycode)
		: KeyEvent(keycode) {}

	EventType getEventType() const override {
		return EventType::KeyPressed;
	}
};

class NWT_API KeyReleasedEvent : public KeyEvent {
public:
	KeyReleasedEvent() = delete;
	KeyReleasedEvent(int keycode)
		: KeyEvent(keycode) {}

	EventType getEventType() const override {
		return EventType::KeyReleased;
	}
};

} // namespace nwt