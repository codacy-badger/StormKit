#pragma once

#include <storm/core/Flags.hpp>

namespace storm::window {
    enum class WindowStyle {
        TitleBar    = 0b1,
        Close       = TitleBar | 0b10,
        Minimizable = TitleBar | 0b100,
        Resizable   = TitleBar | 0b1000,
        Fullscreen  = 0b10000
    };
}

FLAG_ENUM(storm::window::WindowStyle)
