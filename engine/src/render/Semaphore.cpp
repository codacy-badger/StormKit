// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/render/Semaphore.hpp>
#include <storm/engine/vulkan/SemaphoreImpl.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
Semaphore::Semaphore(const Device &device) : m_impl{device} {
}

/////////////////////////////////////
/////////////////////////////////////
Semaphore::~Semaphore() = default;

/////////////////////////////////////
/////////////////////////////////////
Semaphore::Semaphore(Semaphore &&) = default;

/////////////////////////////////////
/////////////////////////////////////
Semaphore &Semaphore::operator=(Semaphore &&) = default;
