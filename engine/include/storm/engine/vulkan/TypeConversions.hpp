// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/Shader.hpp>
#include <storm/engine/render/Types.hpp>
#include <storm/engine/vulkan/UniqueHandle.hpp>

namespace storm::engine {
	vk::Format asVK(storm::engine::ColorFormat type) noexcept;
	storm::engine::ColorFormat fromVK(vk::Format type) noexcept;

	vk::ShaderStageFlagBits asVK(storm::engine::Shader::Stage type) noexcept;
	storm::engine::Shader::Stage fromVK(vk::ShaderStageFlagBits type) noexcept;

	vk::PrimitiveTopology asVK(
	    storm::engine::PrimitiveTopology topology) noexcept;
	storm::engine::PrimitiveTopology fromVK(
	    vk::PrimitiveTopology topology) noexcept;

	vk::PolygonMode            asVK(storm::engine::PolygonMode mode) noexcept;
	storm::engine::PolygonMode fromVK(vk::PolygonMode mode) noexcept;

	vk::CullModeFlags       asVK(storm::engine::CullMode mode) noexcept;
	storm::engine::CullMode fromVK(vk::CullModeFlags mode) noexcept;

	vk::FrontFace            asVK(storm::engine::FrontFace face) noexcept;
	storm::engine::FrontFace fromVK(vk::FrontFace face) noexcept;

	vk::ColorComponentFlags asVK(storm::engine::ColorComponentFlag color) noexcept;
	storm::engine::ColorComponentFlag fromVK(
	    vk::ColorComponentFlags color) noexcept;

	vk::PipelineStageFlagBits asVK(storm::engine::PipelineStage state) noexcept;
	storm::engine::PipelineStage fromVK(
	    vk::PipelineStageFlagBits state) noexcept;

	vk::VertexInputRate asVK(storm::engine::VertexInputRate type) noexcept;
	storm::engine::VertexInputRate fromVK(vk::VertexInputRate type) noexcept;

	vk::Format            asVK(storm::engine::Format type) noexcept;
	storm::engine::Format fromVK(vk::Format type) noexcept;

	vk::CompareOp asVK(storm::engine::CompareOperation op) noexcept;
	storm::engine::CompareOperation fromVK(vk::CompareOp op) noexcept;

	vk::BufferUsageFlags asVK(storm::engine::BufferUsageFlag usage) noexcept;
	storm::engine::BufferUsageFlag fromVK(vk::BufferUsageFlags op) noexcept;

    vk::ImageUsageFlags asVK(storm::engine::ImageUsageFlag usage) noexcept;

    vk::ImageLayout asVK(storm::engine::ImageLayout layout) noexcept;
    storm::engine::ImageLayout fromVK(vk::ImageLayout layout) noexcept;

    vk::ImageAspectFlags asVK(storm::engine::ImageAspectFlag aspect) noexcept;
}
