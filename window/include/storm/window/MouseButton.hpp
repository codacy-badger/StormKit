#pragma once

#include <string>

namespace storm::window {
	enum class MouseButton { UNKNOW, LEFT, RIGHT, MIDDLE, XBUTTON1, XBUTTON2 };
}

namespace storm::core::_private {
	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(storm::window::MouseButton value) {
		using namespace storm::window;

		switch (value) {
		case MouseButton::LEFT:
			return "LEFT";
		case MouseButton::RIGHT:
			return "RIGHT";
		case MouseButton::MIDDLE:
			return "MIDDLE";
		case MouseButton::XBUTTON1:
			return "XBUTTON1";
		case MouseButton::XBUTTON2:
			return "XBUTTON2";
		case MouseButton::UNKNOW:
			return "UNKNOW";
		}
	}
}
