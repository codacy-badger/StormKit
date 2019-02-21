#pragma once

#include <storm/window/Key.hpp>
#include <storm/window/MouseButton.hpp>
#include <xcb/xcb.h>

storm::window::Key x11keyToStormKey(xcb_keysym_t key);
storm::window::MouseButton x11MouseButtonToStormMouseButton(xcb_button_t button);
