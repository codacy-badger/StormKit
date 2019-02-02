#pragma once

#include "RenderWindow.hpp"

namespace storm::engine {
	inline Renderer &RenderWindow::renderer() noexcept {
		return *m_renderer;
	}
}
