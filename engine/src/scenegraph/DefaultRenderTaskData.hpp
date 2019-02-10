// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/CommandBuffer.hpp>
#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/render/Framebuffer.hpp>

#include <storm/engine/graphics/ResourceBase.hpp>

namespace storm::engine {
	struct BeginTaskData {
		RenderPass::Ptr    render_pass;
		Framebuffer::Ptr   framebuffer;

		ResourceBase::ID camera_buffer;
		ResourceBase::ID backbuffer;
	};

	struct ForwardRenderTaskData {
		Framebuffer   *framebuffer;

		RenderPass::Ptr render_pass;

		ResourceBase::ID camera_buffer;
		ResourceBase::ID backbuffer;
	};

	struct SubmitTaskData {
		Framebuffer   *framebuffer;

		ResourceBase::ID backbuffer;
	};
}
