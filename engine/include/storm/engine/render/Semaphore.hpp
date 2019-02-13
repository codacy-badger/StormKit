// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Memory.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/Pimpl.hpp>
#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>

namespace storm::engine {
	class SemaphoreImpl;
	class Semaphore {
	public:
		Unique_Object(Semaphore) explicit Semaphore(const Device &device);
		~Semaphore();

		Semaphore(Semaphore &&);
		Semaphore &operator=(Semaphore &&);

		IMPLEMENTATION(SemaphoreImpl)
	private:
		core::Pimpl<SemaphoreImpl> m_impl;
	};
}
