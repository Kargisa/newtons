#pragma once

#include "../pch.hpp"

namespace nwt {
class Event {
public:
	enum class EventType {
		WindowResized, WindowClosed,
		KeyPressed, KeyReleased, 
		MouseMoved, MousePressed, MouseReleased, MouseScrolled
	};

public:
	Event() = delete;
public:
	virtual constexpr EventType getEventType() = 0;
};
} // namespace nwt