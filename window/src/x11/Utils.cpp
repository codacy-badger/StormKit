#include "Utils.hpp"

#include <X11/keysym.h>

/////////////////////////////////////
/////////////////////////////////////
storm::window::Key x11keyToStormKey(xcb_keysym_t key) {
	using namespace storm::window;

	switch(key) {
		case XK_a:            return Key::A;
		case XK_b:            return Key::B;
		case XK_c:            return Key::C;
		case XK_d:            return Key::D;
		case XK_e:            return Key::E;
		case XK_f:            return Key::F;
		case XK_g:            return Key::G;
		case XK_h:            return Key::H;
		case XK_i:            return Key::I;
		case XK_j:            return Key::J;
		case XK_k:            return Key::K;
		case XK_l:            return Key::L;
		case XK_m:            return Key::M;
		case XK_n:            return Key::N;
		case XK_o:            return Key::O;
		case XK_p:            return Key::P;
		case XK_q:            return Key::Q;
		case XK_r:            return Key::R;
		case XK_s:            return Key::S;
		case XK_t:            return Key::T;
		case XK_u:            return Key::U;
		case XK_v:            return Key::V;
		case XK_w:            return Key::W;
		case XK_x:            return Key::X;
		case XK_y:            return Key::Y;
		case XK_z:            return Key::Z;
		case XK_0:            return Key::NUM0;
		case XK_1:            return Key::NUM1;
		case XK_2:            return Key::NUM2;
		case XK_3:            return Key::NUM3;
		case XK_4:            return Key::NUM4;
		case XK_5:            return Key::NUM5;
		case XK_6:            return Key::NUM6;
		case XK_7:            return Key::NUM7;
		case XK_8:            return Key::NUM8;
		case XK_9:            return Key::NUM9;
		case XK_Escape:       return Key::ESCAPE;
		case XK_Control_L:    return Key::LCONTROL;
		case XK_Shift_L:      return Key::LSHIFT;
		case XK_Alt_L:        return Key::LALT;
		case XK_Super_L:      return Key::LSYSTEM;
		case XK_Control_R:    return Key::RCONTROL;
		case XK_Shift_R:      return Key::RSHIFT;
		case XK_Alt_R:        return Key::RALT;
		case XK_Super_R:      return Key::RSYSTEM;
		case XK_Menu:         return Key::MENU;
		case XK_bracketleft:  return Key::LBRACKET;
		case XK_bracketright: return Key::RBRACKET;
		case XK_semicolon:    return Key::SEMICOLON;
		case XK_comma:        return Key::COMMA;
		case XK_period:       return Key::PERIOD;
		case XK_quoteright:  [[fallthrough]];
		case XK_quoteleft:   return Key::QUOTE;
		case XK_slash:       return Key::SLASH;
		case XK_backslash:   return Key::BACKSLASH;
		case XK_dead_grave:  return Key::TILDE;
		case XK_equal:       return Key::EQUAL;
		case XK_hyphen:      return Key::HYPHEN;
		case XK_space:       return Key::SPACE;
		case XK_Return:      return Key::ENTER;
		case XK_BackSpace:   return Key::BACKSPACE;
		case XK_Tab:         return Key::TAB;
		case XK_Page_Up:     return Key::PAGEUP;
		case XK_Page_Down:   return Key::PAGEDOWN;
		case XK_Begin:       return Key::BEGIN;
		case XK_End:         return Key::END;
		case XK_Home:        return Key::HOME;
		case XK_Insert:      return Key::INSERT;
		case XK_Delete:      return Key::DELETE;
		case XK_KP_Add:      return Key::ADD;
		case XK_KP_Subtract: return Key::SUBSTRACT;
		case XK_KP_Multiply: return Key::MULTIPLY;
		case XK_KP_Divide:   return Key::DIVIDE;
		case XK_Left:        return Key::LEFT;
		case XK_Right:       return Key::RIGHT;
		case XK_Up:          return Key::UP;
		case XK_Down:        return Key::DOWN;
		case XK_KP_0:        return Key::NUMPAD0;
		case XK_KP_1:        return Key::NUMPAD1;
		case XK_KP_2:        return Key::NUMPAD2;
		case XK_KP_3:        return Key::NUMPAD3;
		case XK_KP_4:        return Key::NUMPAD4;
		case XK_KP_5:        return Key::NUMPAD5;
		case XK_KP_6:        return Key::NUMPAD6;
		case XK_KP_7:        return Key::NUMPAD7;
		case XK_KP_8:        return Key::NUMPAD8;
		case XK_KP_9:        return Key::NUMPAD9;
		case XK_F1:          return Key::F1;
		case XK_F2:          return Key::F2;
		case XK_F3:          return Key::F3;
		case XK_F4:          return Key::F4;
		case XK_F5:          return Key::F5;
		case XK_F6:          return Key::F6;
		case XK_F7:          return Key::F7;
		case XK_F8:          return Key::F8;
		case XK_F9:          return Key::F9;
		case XK_F10:         return Key::F10;
		case XK_F11:         return Key::F11;
		case XK_F12:         return Key::F12;
		case XK_F13:         return Key::F13;
		case XK_F14:         return Key::F14;
		case XK_F15:         return Key::F15;
		case XK_Pause:       return Key::PAUSE;
	}

	return Key::UNKNOW;
}

/////////////////////////////////////
/////////////////////////////////////
storm::window::MouseButton x11MouseButtonToStormMouseButton(xcb_button_t button) {
	switch(button) {
		case XCB_BUTTON_INDEX_1: return storm::window::MouseButton::LEFT;
		case XCB_BUTTON_INDEX_2: return storm::window::MouseButton::MIDDLE;
		case XCB_BUTTON_INDEX_3: return storm::window::MouseButton::RIGHT;
		case XCB_BUTTON_INDEX_4: return storm::window::MouseButton::XBUTTON1;
		case XCB_BUTTON_INDEX_5: return storm::window::MouseButton::XBUTTON2;
	}

	return storm::window::MouseButton::UNKNOW;
}
