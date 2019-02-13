#pragma once

#include <cstdint>
#include <storm/window/Key.hpp>
#include <storm/window/MouseButton.hpp>

namespace storm::window {
	enum class EventType {
		None,
		Closed,
		Resized,
		Minimized,
		Maximized,
		KeyPressed,
		KeyReleased,
		MouseButtonPushed,
		MouseButtonReleased,
		MouseEntered,
		MouseExited,
		MouseMoved,
		LostFocus,
		GainedFocus
	};
	struct Event {
		struct ResizedEvent {
			std::uint16_t width;
			std::uint16_t height;
		};
		struct KeyEvent {
			Key key;
		};
		struct MouseEvent {
			MouseButton  button;
			std::int16_t x;
			std::int16_t y;
		};
		EventType type = EventType::None;
		union {
			ResizedEvent resizedEvent;
			KeyEvent     keyEvent;
			MouseEvent   mouseEvent;
			// MouseMoved       mouseMoved;
		};
	};
}
