// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Pimpl.hpp>
#include <storm/window/Window.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>
#include <storm/engine/render/FrameToken.hpp>

namespace storm::engine {
	class SurfaceImpl;
	class Surface {
	public:
		explicit Surface(const Device &device, const window::Window &window);
		~Surface();

		Surface(Surface &&);
		Surface &operator=(Surface &&);

		FrameToken nextFrame();
		void present(Framebuffer &framebuffer, const FrameToken &token);

		IMPLEMENTATION(SurfaceImpl)
	private:
		core::Pimpl<SurfaceImpl> m_impl;
	};
}
