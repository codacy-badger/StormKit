// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/RenderTaskBuilder.hpp>
#include <storm/engine/graphics/Resource.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBuilder::RenderTaskBuilder(
    const Device &device, RenderTaskBase &task, ResourcePool &pool)
    : m_device {device}, m_task {task}, m_pool {pool} {}

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBuilder::~RenderTaskBuilder() = default;

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBuilder::RenderTaskBuilder(RenderTaskBuilder &&) = default;

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ID RenderTaskBuilder::createAttachment(
    std::string name, Texture::Description description) {
	return create<TextureResource>(
	    std::move(name), std::move(description), m_task.m_create_attachments);
}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ID RenderTaskBuilder::writeAttachment(
    ResourceBase::ID attachment) {
	return write<TextureResource>(attachment, m_task.m_write_attachments);
}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ID RenderTaskBuilder::readAttachment(
    ResourceBase::ID attachment) {
	return read<TextureResource>(attachment, m_task.m_read_attachments);
}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ID RenderTaskBuilder::createUniformBuffer(
    std::string name, UniformBuffer::Description description) {
	return create<UniformBufferResource>(
	    std::move(name), std::move(description), m_task.m_create_buffers);
}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ID RenderTaskBuilder::writeUniformBuffer(
    ResourceBase::ID buffer) {
	return write<UniformBufferResource>(buffer, m_task.m_write_buffers);
}

/////////////////////////////////////
/////////////////////////////////////
ResourceBase::ID RenderTaskBuilder::readUniformBuffer(ResourceBase::ID buffer) {
	return read<UniformBufferResource>(buffer, m_task.m_read_buffers);
}
