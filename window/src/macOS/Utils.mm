#include "Utils.h"

#include <iostream>

#include <AppKit/NSEvent.h>
#include <Carbon/Carbon.h>

storm::window::MouseButton toStormMouseButton(int button) {
	using namespace storm::window;

	switch(button) {
		case 0: return MouseButton::LEFT;
		case 1: return MouseButton::RIGHT;
		case 2: return MouseButton::MIDDLE;
		case 3: return MouseButton::XBUTTON1;
		case 4: return MouseButton::XBUTTON2;
	}
}

storm::window::Key localizedKeyToStormKey(char code) {
	using namespace storm::window;

	switch(code) {
		case 'a':
		case 'A': return Key::A;
		case 'b':
		case 'B': return Key::B;
		case 'c':
		case 'C': return Key::C;
		case 'd':
		case 'D': return Key::D;
		case 'e':
		case 'E': return Key::E;
		case 'f':
		case 'F': return Key::F;
		case 'g':
		case 'G': return Key::G;
		case 'h':
		case 'H': return Key::H;
		case 'i':
		case 'I': return Key::I;
		case 'j':
		case 'J': return Key::J;
		case 'k':
		case 'K': return Key::K;
		case 'l':
		case 'L': return Key::L;
		case 'm':
		case 'M': return Key::M;
		case 'n':
		case 'N': return Key::N;
		case 'o':
		case 'O': return Key::O;
		case 'p':
		case 'P': return Key::P;
		case 'q':
		case 'Q': return Key::Q;
		case 'r':
		case 'R': return Key::R;
		case 's':
		case 'S': return Key::S;
		case 't':
		case 'T': return Key::T;
		case 'u':
		case 'U': return Key::U;
		case 'v':
		case 'V': return Key::V;
		case 'w':
		case 'W': return Key::W;
		case 'x':
		case 'X': return Key::X;
		case 'y':
		case 'Y': return Key::Y;
		case 'z':
		case 'Z': return Key::Z;
	}

	return Key::UNKNOW;
}

storm::window::Key nonLocalizedKeytoStormKey(unsigned short code) {
	using namespace storm::window;

	switch(code) {
		case kVK_ANSI_A:              return Key::A;
		case kVK_ANSI_B:              return Key::B;
		case kVK_ANSI_C:              return Key::C;
		case kVK_ANSI_D:              return Key::D;
		case kVK_ANSI_E:              return Key::E;
		case kVK_ANSI_F:              return Key::F;
		case kVK_ANSI_G:              return Key::G;
		case kVK_ANSI_H:              return Key::H;
		case kVK_ANSI_I:              return Key::I;
		case kVK_ANSI_J:              return Key::J;
		case kVK_ANSI_K:              return Key::K;
		case kVK_ANSI_L:              return Key::L;
		case kVK_ANSI_M:              return Key::M;
		case kVK_ANSI_N:              return Key::N;
		case kVK_ANSI_O:              return Key::O;
		case kVK_ANSI_P:              return Key::P;
		case kVK_ANSI_Q:              return Key::Q;
		case kVK_ANSI_R:              return Key::R;
		case kVK_ANSI_S:              return Key::S;
		case kVK_ANSI_T:              return Key::T;
		case kVK_ANSI_U:              return Key::U;
		case kVK_ANSI_V:              return Key::V;
		case kVK_ANSI_W:              return Key::W;
		case kVK_ANSI_X:              return Key::X;
		case kVK_ANSI_Y:              return Key::Y;
		case kVK_ANSI_Z:              return Key::Z;

		case kVK_ANSI_0:              return Key::NUM0;
		case kVK_ANSI_1:              return Key::NUM1;
		case kVK_ANSI_2:              return Key::NUM2;
		case kVK_ANSI_3:              return Key::NUM3;
		case kVK_ANSI_4:              return Key::NUM4;
		case kVK_ANSI_5:              return Key::NUM5;
		case kVK_ANSI_6:              return Key::NUM6;
		case kVK_ANSI_7:              return Key::NUM7;
		case kVK_ANSI_8:              return Key::NUM8;
		case kVK_ANSI_9:              return Key::NUM9;

		case kVK_ANSI_Keypad0:        return Key::NUMPAD0;
		case kVK_ANSI_Keypad1:        return Key::NUMPAD1;
		case kVK_ANSI_Keypad2:        return Key::NUMPAD2;
		case kVK_ANSI_Keypad3:        return Key::NUMPAD3;
		case kVK_ANSI_Keypad4:        return Key::NUMPAD4;
		case kVK_ANSI_Keypad5:        return Key::NUMPAD5;
		case kVK_ANSI_Keypad6:        return Key::NUMPAD6;
		case kVK_ANSI_Keypad7:        return Key::NUMPAD7;
		case kVK_ANSI_Keypad8:        return Key::NUMPAD8;
		case kVK_ANSI_Keypad9:        return Key::NUMPAD9;

		case kVK_ANSI_KeypadPlus:     return Key::ADD;
		case kVK_ANSI_KeypadMinus:    return Key::SUBSTRACT;
		case kVK_ANSI_KeypadMultiply: return Key::MULTIPLY;
		case kVK_ANSI_KeypadDivide:   return Key::DIVIDE;
		case kVK_ANSI_KeypadEquals:
		case kVK_ANSI_Equal:          return Key::EQUAL;

		case kVK_ANSI_Slash:          return Key::SLASH;
		case kVK_ANSI_Backslash:      return Key::BACKSLASH;
		case kVK_ANSI_Minus:          return Key::HYPHEN;
		case kVK_ANSI_KeypadDecimal:
		case kVK_ANSI_Period:         return Key::PERIOD;
		case kVK_ANSI_Comma:          return Key::COMMA;
		case kVK_ANSI_Semicolon:      return Key::SEMICOLON;
		case kVK_ANSI_Quote:          return Key::QUOTE;
		case kVK_ANSI_LeftBracket:    return Key::LBRACKET;
		case kVK_ANSI_RightBracket:   return Key::RBRACKET;
		case kVK_Tab:                 return Key::TAB;
		case kVK_ANSI_KeypadEnter:
		case kVK_Return:              return Key::ENTER;
		case kVK_Space:               return Key::SPACE;
		case kVK_ISO_Section:         return Key::TILDE;

		case NSF1FunctionKey:
		case kVK_F1:                  return Key::F1;
		case NSF2FunctionKey:
		case kVK_F2:                  return Key::F2;
		case NSF3FunctionKey:
		case kVK_F3:                  return Key::F3;
		case NSF4FunctionKey:
		case kVK_F4:                  return Key::F4;
		case NSF5FunctionKey:
		case kVK_F5:                  return Key::F5;
		case NSF6FunctionKey:
		case kVK_F6:                  return Key::F6;
		case NSF7FunctionKey:
		case kVK_F7:                  return Key::F7;
		case NSF8FunctionKey:
		case kVK_F8:                  return Key::F8;
		case NSF9FunctionKey:
		case kVK_F9:                  return Key::F9;
		case NSF10FunctionKey:
		case kVK_F10:                 return Key::F10;
		case NSF11FunctionKey:
		case kVK_F11:                 return Key::F11;
		case NSF12FunctionKey:
		case kVK_F12:                 return Key::F12;
		case NSF13FunctionKey:
		case kVK_F13:                 return Key::F13;
		case NSF14FunctionKey:
		case kVK_F14:                 return Key::F14;
		case NSF15FunctionKey:
		case kVK_F15:                 return Key::F15;

		case NSUpArrowFunctionKey:
		case kVK_UpArrow:             return Key::UP;
		case NSDownArrowFunctionKey:
		case kVK_DownArrow:           return Key::DOWN;
		case NSLeftArrowFunctionKey:
		case kVK_LeftArrow:           return Key::LEFT;
		case NSRightArrowFunctionKey:
		case kVK_RightArrow:          return Key::RIGHT;

		case kVK_Control:             return Key::LCONTROL;
		case kVK_RightControl:        return Key::RCONTROL;
		case kVK_Shift:               return Key::LSHIFT;
		case kVK_RightShift:          return Key::RSHIFT;
		case kVK_Option:              return Key::LALT;
		case kVK_RightOption:         return Key::RALT;
		case kVK_Command:             return Key::LSYSTEM;
		case kVK_RightCommand:        return Key::RSYSTEM;

		case kVK_Escape:              return Key::ESCAPE;

		case NSMenuFunctionKey:
		case 0x7f:                    return Key::MENU;
		case NSPageUpFunctionKey:
		case kVK_PageUp:              return Key::PAGEUP;
		case NSPageDownFunctionKey:
		case kVK_PageDown:            return Key::PAGEDOWN;
		case NSEndFunctionKey:
		case kVK_End:                 return Key::END;
		case NSHomeFunctionKey:
		case kVK_Home:                return Key::HOME;
		case NSInsertFunctionKey:
		case kVK_Help:                return Key::INSERT;
		case NSDeleteFunctionKey:
		case kVK_ForwardDelete:       return Key::DELETE;

		case NSBeginFunctionKey:      return Key::BEGIN;
		case NSPauseFunctionKey:      return Key::PAUSE;
		default:                      return Key::UNKNOW;
	}

	return Key::UNKNOW;
}

storm::window::VideoSettings::Size toStormVec(NSPoint point) {
	return {static_cast<std::uint16_t>(point.x), static_cast<std::uint16_t>(point.y)};
}

NSPoint fromStormVec(const storm::window::VideoSettings::Size &point) {
	return CGPointMake(point.w, point.h);
}

storm::window::VideoSettings::Size toStormVec(CGSize size) {
	return {static_cast<std::uint16_t>(size.width), static_cast<std::uint16_t>(size.height)};
}
