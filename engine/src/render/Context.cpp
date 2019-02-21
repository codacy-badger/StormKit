// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/Context.hpp>
#include <storm/engine/vulkan/ContextImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
Context::Context(ContextSettings settings) : m_impl{settings} {
}

/////////////////////////////////////
/////////////////////////////////////
Context::~Context() = default;

/////////////////////////////////////
/////////////////////////////////////
Context::Context(Context &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Context &Context::operator=(Context &&) = default;

/////////////////////////////////////
/////////////////////////////////////
PhysicalDevice Context::bestPhysicalDevice() {
    return m_impl->bestPhysicalDevice();
}

/////////////////////////////////////
/////////////////////////////////////
const ContextSettings &Context::settings() {
    return m_impl->settings();
}
