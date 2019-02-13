// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/graphics/ResourceBase.hpp>
#include <storm/engine/render/CommandBuffer.hpp>
#include <storm/engine/render/Framebuffer.hpp>
#include <storm/engine/render/RenderPass.hpp>

namespace storm::engine {
	struct BeginTaskData {
		ResourceBase::ID meshdata_buffer;
		ResourceBase::ID camera_buffer;
		ResourceBase::ID backbuffer;
	};

	struct ForwardRenderTaskData {
		ResourceBase::ID meshdata_buffer;
		ResourceBase::ID camera_buffer;
		ResourceBase::ID backbuffer;
	};

	struct SubmitTaskData {
		ResourceBase::ID meshdata_buffer;
		ResourceBase::ID backbuffer;
	};
}
