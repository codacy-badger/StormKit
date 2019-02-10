// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/PhysicalDevice.hpp>
#include <storm/engine/render/Shader.hpp>
#include <storm/engine/render/Program.hpp>
#include <storm/engine/render/RenderPass.hpp>
#include <storm/engine/render/CommandBuffer.hpp>
#include <storm/engine/render/VertexBuffer.hpp>
#include <storm/engine/render/IndexBuffer.hpp>
#include <storm/engine/render/UniformBuffer.hpp>
#include <storm/engine/render/Fence.hpp>
#include <storm/engine/render/Semaphore.hpp>
#include <storm/engine/render/Texture.hpp>

#include <storm/window/Window.hpp>

#include <storm/core/Pimpl.hpp>
#include <storm/core/NonCopyable.hpp>

namespace storm::engine {
	class DeviceImpl;
	class Device : public core::NonCopyable {
		public:
			explicit Device(const Context &context, window::NativeHandle handle, PhysicalDevice &&device);
			~Device();
			
			Device(Device &&);
			Device &operator=(Device &&);

			ColorFormat bestDepthFormat() const noexcept;

			Program createProgram() const;
			Program::Ptr createProgramPtr() const;

			Shader createShader(Shader::DefaultShader shader) const;
			Shader createShader(Shader::Stage stage, const _std::filesystem::path &path) const;
			Shader createShader(Shader::Stage stage, const std::byte* data, std::size_t size) const;
			Shader::Ptr createShaderPtr(Shader::DefaultShader shader) const;
			Shader::Ptr createShaderPtr(Shader::Stage stage, const _std::filesystem::path &path) const;
			Shader::Ptr createShaderPtr(Shader::Stage stage, const std::byte* data, std::size_t size) const;
			
			RenderPass createRenderPass(bool default_subpass = false, bool enable_depth_attachment = false) const;
			RenderPass::Ptr createRenderPassPtr(bool default_subpass = false, bool enable_depth_attachment = false) const;

			VertexBuffer createVertexBuffer(std::size_t size, std::size_t alignement) const;
			VertexBuffer::Ptr createVertexBufferPtr(std::size_t size, std::size_t alignement) const;
			IndexBuffer createIndexBuffer(std::size_t size, std::size_t alignement) const;
			IndexBuffer::Ptr createIndexBufferPtr(std::size_t size, std::size_t alignement) const;
			UniformBuffer createUniformBuffer(UniformBuffer::Description description) const;
			UniformBuffer::Ptr createUniformBufferPtr(UniformBuffer::Description description) const;

			Fence createFence() const;
			Fence::Ptr createFencePtr() const;

			Semaphore createSemaphore() const;
			Semaphore::Ptr createSemaphorePtr() const;

			CommandBuffer createCommandBuffer() const;
			CommandBuffer::Ptr createCommandBufferPtr() const;
			
			Texture createTexture(const storm::image::Image &image) const;
			Texture createTexture(Texture::Description description) const;
			Texture::Ptr createTexturePtr(const storm::image::Image &image) const;
			Texture::Ptr createTexturePtr(Texture::Description description) const;

			void waitIdle() const;

			uvec2 maxImage2DSize() const;

			IMPLEMENTATION(DeviceImpl)
		private:
			core::Pimpl<DeviceImpl> m_impl;
	};
}
