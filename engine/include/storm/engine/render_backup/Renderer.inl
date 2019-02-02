#pragma once

#include "Renderer.hpp"

namespace storm::engine {
	inline const window::VideoSettings &Renderer::videoSettings() const noexcept {
		return m_video_settings;
	}

	inline const AppSettings &Renderer::appSettings() const noexcept {
		return m_app_settings;
	}

	inline const RenderPass &Renderer::defaultRenderPass() const noexcept {
		return *m_default_render_pass;
	}

	inline const mat4 &Renderer::currentViewMatrix() const noexcept {
		return m_matrix_cache.view;
	}

	inline const mat4 &Renderer::currentProjectionMatrix() const noexcept {
		return m_matrix_cache.projection;
	}

	inline void Renderer::setViewMatrix(const mat4 &matrix) noexcept {
		m_matrix_cache.view = matrix;

		m_matrices_updated = true;
	}

	inline void Renderer::setProjectionMatrix(const mat4 &matrix) noexcept {
		m_matrix_cache.projection = matrix;

		m_matrices_updated = true;
	}
}
