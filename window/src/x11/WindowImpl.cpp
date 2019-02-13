#include "WindowImpl.hpp"

#include "Utils.hpp"

#include <cstddef>
#include <stdexcept>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_keysyms.h>

using namespace storm::window;

/////////////////////////////////////
/////////////////////////////////////
WindowImpl::WindowImpl() = default;

/////////////////////////////////////
/////////////////////////////////////
WindowImpl::~WindowImpl() {
	if (m_key_symbols)
		xcb_key_symbols_free(m_key_symbols);

	if (m_window) {
		xcb_destroy_window(m_connection.get(), m_window);
		xcb_flush(m_connection.get());
	}
}

/////////////////////////////////////
/////////////////////////////////////
WindowImpl::WindowImpl(
    const std::string &title, const VideoSettings &settings, WindowStyle style)
    : m_window(0), m_key_symbols(nullptr), m_is_open(false),
      m_is_visible(false) {
	create(title, settings, style);
}

/////////////////////////////////////
/////////////////////////////////////
void WindowImpl::create(const std::string &title, const VideoSettings &settings,
    WindowStyle style) {
	static constexpr const auto EVENTS
	    = XCB_EVENT_MASK_FOCUS_CHANGE | XCB_EVENT_MASK_BUTTON_PRESS
	      | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_BUTTON_MOTION
	      | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_KEY_PRESS
	      | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_STRUCTURE_NOTIFY
	      | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW
	      | XCB_EVENT_MASK_VISIBILITY_CHANGE | XCB_EVENT_MASK_PROPERTY_CHANGE
	      | XCB_EVENT_MASK_EXPOSURE;

	m_connection.reset(xcb_connect(nullptr, nullptr));

	auto screen
	    = xcb_setup_roots_iterator(xcb_get_setup(m_connection.get())).data;
	m_window = xcb_generate_id(m_connection.get());

	std::uint32_t value_list[]
	    = {screen->black_pixel, screen->black_pixel, EVENTS};
	xcb_create_window(m_connection.get(), XCB_COPY_FROM_PARENT, m_window,
	    screen->root, 0, 0, settings.size.w, settings.size.h, 0,
	    XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
	    XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXEL | XCB_CW_EVENT_MASK,
	    value_list);

	xcb_map_window(m_connection.get(), m_window);

	// init key_symbol map, this is needed to extract the keysymbol from event
	m_key_symbols = xcb_key_symbols_alloc(m_connection.get());

	// set the window name
	setTitle(title);

	// set the window's style
	if ((style & WindowStyle::Fullscreen) == WindowStyle::Fullscreen) {

	} else {
		// checking if the window manager support window decoration
		static const auto window_manager_hints_str
		    = std::string_view("_MOTIF_WM_HINTS");

		const auto atom_request = xcb_intern_atom(m_connection.get(), 0,
		    window_manager_hints_str.length(), window_manager_hints_str.data());
		const auto atom_reply
		    = xcb_intern_atom_reply(m_connection.get(), atom_request, nullptr);

		if (atom_reply) {
			static constexpr const auto MWM_HINTS_FUNCTIONS   = 1 << 0;
			static constexpr const auto MWM_HINTS_DECORATIONS = 1 << 1;

			static constexpr const auto MWM_DECOR_BORDER   = 1 << 1;
			static constexpr const auto MWM_DECOR_RESIZE   = 1 << 2;
			static constexpr const auto MWM_DECOR_TITLE    = 1 << 3;
			static constexpr const auto MWM_DECOR_MENU     = 1 << 4;
			static constexpr const auto MWM_DECOR_MINIMIZE = 1 << 5;
			static constexpr const auto MWM_DECOR_MAXIMIZE = 1 << 6;

			static constexpr const auto MWM_FUNC_RESIZE   = 1 << 1;
			static constexpr const auto MWM_FUNC_MOVE     = 1 << 2;
			static constexpr const auto MWM_FUNC_MINIMIZE = 1 << 3;
			static constexpr const auto MWM_FUNC_MAXIMIZE = 1 << 4;
			static constexpr const auto MWM_FUNC_CLOSE    = 1 << 5;

			struct {
				uint32_t flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
				uint32_t functions   = 0;
				uint32_t decorations = 0;
				int32_t  input_mode  = 0;
				uint32_t state       = 0;
			} hints;

			if ((style & WindowStyle::TitleBar) == WindowStyle::TitleBar) {
				hints.decorations
				    |= MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MENU;
				hints.functions |= MWM_FUNC_MOVE;
			}

			if ((style & WindowStyle::Close) == WindowStyle::Minimizable) {
				hints.decorations |= 0;
				hints.functions |= MWM_FUNC_CLOSE;
			}

			if ((style & WindowStyle::Minimizable)
			    == WindowStyle::Minimizable) {
				hints.decorations |= MWM_DECOR_MINIMIZE;
				hints.functions |= MWM_FUNC_MINIMIZE;
			}

			if ((style & WindowStyle::Resizable) == WindowStyle::Resizable) {
				hints.decorations |= MWM_DECOR_RESIZE | MWM_DECOR_MAXIMIZE;
				hints.functions |= MWM_FUNC_RESIZE | MWM_FUNC_MAXIMIZE;
			}

			// applying hints
			xcb_change_property(m_connection.get(), XCB_PROP_MODE_REPLACE,
			    m_window, atom_reply->atom, XCB_ATOM_WM_HINTS, 32, 5,
			    reinterpret_cast<std::byte *>(&hints));

			free(atom_reply);
		}

		// hack to force some windows managers to disable resizing
		if ((style & WindowStyle::Resizable) != WindowStyle::Resizable) {
			auto size_hints  = xcb_size_hints_t {};
			size_hints.flags = XCB_ICCCM_SIZE_HINT_P_MIN_SIZE
			                   | XCB_ICCCM_SIZE_HINT_P_MAX_SIZE;
			size_hints.min_width = size_hints.max_width = settings.size.w;
			size_hints.min_height = size_hints.max_height = settings.size.h;
			xcb_icccm_set_wm_normal_hints(
			    m_connection.get(), m_window, &size_hints);
		}
	}

	xcb_flush(m_connection.get());

	m_is_open    = true;
	m_is_visible = true;

	handles.window     = m_window;
	handles.connection = m_connection.get();
}

/////////////////////////////////////
/////////////////////////////////////
void WindowImpl::close() noexcept {
	m_is_open    = false;
	m_is_visible = false;
}

/////////////////////////////////////
/////////////////////////////////////
void WindowImpl::display() noexcept {}

/////////////////////////////////////
/////////////////////////////////////
bool WindowImpl::pollEvent(Event &event, void *native_event) noexcept {
	xcb_generic_event_t *xevent;
	if (xevent = xcb_poll_for_event(m_connection.get()); !xevent)
		return false;

	processEvents(*xevent, native_event);

	return AbstractWindow::pollEvent(event, native_event);
}

/////////////////////////////////////
/////////////////////////////////////
bool WindowImpl::waitEvent(Event &event, void *native_event) noexcept {
	xcb_generic_event_t *xevent;
	if (xevent = xcb_poll_for_event(m_connection.get()); !xevent)
		return false;

	processEvents(*xevent, native_event);

	return AbstractWindow::waitEvent(event, native_event);
}

/////////////////////////////////////
/////////////////////////////////////
void WindowImpl::setTitle(const std::string &title) noexcept {
	xcb_change_property(m_connection.get(), XCB_PROP_MODE_REPLACE, m_window,
	    XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
	    static_cast<std::uint32_t>(title.length()), title.c_str());
}

/////////////////////////////////////
/////////////////////////////////////
void WindowImpl::setVideoSettings(
    const storm::window::VideoSettings &settings) noexcept {}

/////////////////////////////////////
/////////////////////////////////////
VideoSettings::Size WindowImpl::size() const noexcept {
	const auto reply = xcb_get_geometry_reply(m_connection.get(),
	    xcb_get_geometry(m_connection.get(), m_window), nullptr);

	auto result = VideoSettings::Size {reply->width, reply->height};
	free(reply);

	return result;
}

/////////////////////////////////////
/////////////////////////////////////
bool WindowImpl::isOpen() const noexcept { return m_is_open; }

/////////////////////////////////////
/////////////////////////////////////
bool WindowImpl::isVisible() const noexcept { return m_is_visible; }

/////////////////////////////////////
/////////////////////////////////////
void WindowImpl::setMousePosition(std::int16_t x, std::int16_t y) const
    noexcept {
	m_mouse_x = x;
	m_mouse_y = y;

	xcb_warp_pointer(m_connection.get(), 0, m_window, 0, 0, 0, 0, x, y);

	xcb_flush(m_connection.get());
}

/////////////////////////////////////
/////////////////////////////////////
storm::window::NativeHandle WindowImpl::nativeHandle() const noexcept {
	return reinterpret_cast<storm::window::NativeHandle>(
	    const_cast<Handles *>(&handles));
}

/////////////////////////////////////
/////////////////////////////////////
void WindowImpl::processEvents(xcb_generic_event_t xevent, void *native_event) {
	bool ignore = false;
	switch (xevent.response_type & ~0x80) {
	case XCB_KEY_PRESS: {
		auto keyboard_event
		    = reinterpret_cast<xcb_key_press_event_t *>(&xevent);

		auto symbol
		    = xcb_key_press_lookup_keysym(m_key_symbols, keyboard_event, 0);

		auto key = x11keyToStormKey(symbol);

		AbstractWindow::keyDownEvent(key);
		break;
	}
	case XCB_KEY_RELEASE: {
		auto keyboard_event
		    = reinterpret_cast<xcb_key_release_event_t *>(&xevent);

		auto symbol
		    = xcb_key_release_lookup_keysym(m_key_symbols, keyboard_event, 0);

		auto key = x11keyToStormKey(symbol);

		AbstractWindow::keyUpEvent(key);
		break;
	}
	case XCB_MOTION_NOTIFY: {
		auto mouse_event
		    = reinterpret_cast<xcb_motion_notify_event_t *>(&xevent);
		if (m_mouse_x == mouse_event->event_x
		    && m_mouse_y == mouse_event->event_y) {
			ignore = true;
			break;
		}

		m_mouse_x = mouse_event->event_x;
		m_mouse_y = mouse_event->event_y;
		AbstractWindow::mouseMoveEvent(
		    mouse_event->event_x, mouse_event->event_y);
		break;
	}
	case XCB_BUTTON_PRESS: {
		auto button_event
		    = reinterpret_cast<xcb_button_press_event_t *>(&xevent);

		auto button = button_event->detail;
		AbstractWindow::mouseDownEvent(x11MouseButtonToStormMouseButton(button),
		    button_event->event_x, button_event->event_y);
		break;
	}
	case XCB_BUTTON_RELEASE: {
		auto button_event
		    = reinterpret_cast<xcb_button_press_event_t *>(&xevent);

		auto button = button_event->detail;
		AbstractWindow::mouseUpEvent(x11MouseButtonToStormMouseButton(button),
		    button_event->event_x, button_event->event_y);
		break;
	}
	case XCB_ENTER_NOTIFY:
		AbstractWindow::mouseEnteredEvent();
		break;
	case XCB_LEAVE_NOTIFY:
		AbstractWindow::mouseExitedEvent();
		break;
	case XCB_VISIBILITY_NOTIFY:
		/*if(xevent.xvisibility.window == m_window)
		        m_is_visible = xevent.xvisibility.state !=
		   VisibilityFullyObscured;*/
		break;
	case XCB_DESTROY_NOTIFY:
		AbstractWindow::closeEvent();
		break;
		// TODO intercept close resize minimize maximize events
	}

	if (native_event)
		*reinterpret_cast<xcb_generic_event_t *>(native_event) = xevent;
}
