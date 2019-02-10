// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/RenderTaskBuilder.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBuilder::RenderTaskBuilder(const Device &device, RenderTaskBase &task, ResourcePool &pool)
	: m_device{device}, m_task{task}, m_pool{pool} {

}

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBuilder::~RenderTaskBuilder() = default;

/////////////////////////////////////
/////////////////////////////////////
RenderTaskBuilder::RenderTaskBuilder(RenderTaskBuilder &&) = default;
