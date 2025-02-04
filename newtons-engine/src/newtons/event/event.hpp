#pragma once

#include "../pch.hpp"

namespace nwt {
class NWT_API Event {
public:
	enum class EventType {
		WindowResized, WindowClosed,
		KeyPressed, KeyReleased, 
		MouseMoved, MousePressed, MouseReleased, MouseScrolled
	};

protected:
	Event() {}
public:
	virtual EventType getEventType() const = 0;
};
} // namespace nwt