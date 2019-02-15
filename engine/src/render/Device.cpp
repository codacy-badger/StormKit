// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/Device.hpp>
#include <storm/engine/vulkan/DeviceImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
Device::Device(const Context &context, window::NativeHandle handle,
    PhysicalDevice &&device)
    : m_impl {*this, context, handle, std::move(device)} {}

/////////////////////////////////////
/////////////////////////////////////
Device::~Device() = default;

/////////////////////////////////////
/////////////////////////////////////
Device::Device(Device &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Device &Device::operator=(Device &&) = default;

/////////////////////////////////////
/////////////////////////////////////
ColorFormat Device::bestDepthFormat() const noexcept {
	return m_impl->bestDepthFormat();
}

/////////////////////////////////////
/////////////////////////////////////
Program Device::createProgram() const { return Program {*this}; }

/////////////////////////////////////
/////////////////////////////////////
Program::Ptr Device::createProgramPtr() const {
	return Program::makeUnique(*this);
}

/////////////////////////////////////
/////////////////////////////////////
Shader Device::createShader(
    Shader::Stage stage, const _std::filesystem::path &path) const {
	return Shader {*this, stage, path};
}

/////////////////////////////////////
/////////////////////////////////////
Shader Device::createShader(
    Shader::Stage stage, const std::byte *data, std::size_t size) const {
	return Shader {*this, stage, data, size};
}

/////////////////////////////////////
/////////////////////////////////////
Shader::Ptr Device::createShaderPtr(
    Shader::Stage stage, const _std::filesystem::path &path) const {
	return Shader::makeUnique(*this, stage, path);
}

/////////////////////////////////////
/////////////////////////////////////
Shader::Ptr Device::createShaderPtr(
    Shader::Stage stage, const std::byte *data, std::size_t size) const {
	return Shader::makeUnique(*this, stage, data, size);
}

/////////////////////////////////////
/////////////////////////////////////
RenderPass Device::createRenderPass(
    bool default_subpass, bool enable_depth_attachment) const {
	auto render_pass = RenderPass {*this};
	if (!default_subpass)
		return render_pass;

	auto subpass
	    = RenderPass::SubPass {{RenderPass::SubPass::EXTERNAL}, {}, {0}};

	if (enable_depth_attachment)
		subpass.output_attachments.emplace_back(1);

	render_pass.addSubPass(std::move(subpass));

	return render_pass;
}

/////////////////////////////////////
/////////////////////////////////////
RenderPass::Ptr Device::createRenderPassPtr(
    bool default_subpass, bool enable_depth_attachment) const {
	auto render_pass = RenderPass::makeUnique(*this);
	if (!default_subpass)
		return render_pass;

	auto subpass
	    = RenderPass::SubPass {{RenderPass::SubPass::EXTERNAL}, {}, {0}};

	if (enable_depth_attachment)
		subpass.output_attachments.emplace_back(1);

	render_pass->addSubPass(std::move(subpass));

	return render_pass;
}

/////////////////////////////////////
/////////////////////////////////////
Framebuffer Device::createFramebuffer() const {
	return Framebuffer{*this};
}

/////////////////////////////////////
/////////////////////////////////////
Framebuffer::Ptr Device::createFramebufferPtr() const {
	return Framebuffer::makeUnique(*this);
}

/////////////////////////////////////
/////////////////////////////////////
HardwareBuffer Device::createHardwareBuffer(
	HardwareBuffer::Description description) const {
	return HardwareBuffer {*this, std::move(description)};
}

/////////////////////////////////////
/////////////////////////////////////
HardwareBuffer::Ptr Device::createHardwareBufferPtr(
	HardwareBuffer::Description description) const {
	return HardwareBuffer::makeUnique(*this, std::move(description));
}

/////////////////////////////////////
/////////////////////////////////////
Fence Device::createFence() const { return Fence {*this}; }

/////////////////////////////////////
/////////////////////////////////////
Fence::Ptr Device::createFencePtr() const { return Fence::makeUnique(*this); }

/////////////////////////////////////
/////////////////////////////////////
Semaphore Device::createSemaphore() const { return Semaphore {*this}; }

/////////////////////////////////////
/////////////////////////////////////
Semaphore::Ptr Device::createSemaphorePtr() const {
	return Semaphore::makeUnique(*this);
}

/////////////////////////////////////
/////////////////////////////////////
CommandBuffer Device::createCommandBuffer() const {
	return CommandBuffer {*this};
}

/////////////////////////////////////
/////////////////////////////////////
CommandBuffer::Ptr Device::createCommandBufferPtr() const {
	return CommandBuffer::makeUnique(*this);
}

/////////////////////////////////////
/////////////////////////////////////
Texture Device::createTexture(const storm::image::Image &image) const {
	return Texture {*this, image};
}

/////////////////////////////////////
/////////////////////////////////////
Texture Device::createTexture(Texture::Description description) const {
	return Texture {*this, std::move(description)};
}

/////////////////////////////////////
/////////////////////////////////////
Texture::Ptr Device::createTexturePtr(const storm::image::Image &image) const {
	return Texture::makeUnique(*this, image);
}

/////////////////////////////////////
/////////////////////////////////////
Texture::Ptr Device::createTexturePtr(Texture::Description description) const {
	return Texture::makeUnique(*this, std::move(description));
}

/////////////////////////////////////
/////////////////////////////////////
void Device::waitIdle() const { m_impl->waitIdle(); }

/////////////////////////////////////
/////////////////////////////////////
uvec2 Device::maxImage2DSize() const { return m_impl->maxImage2DSize(); }
