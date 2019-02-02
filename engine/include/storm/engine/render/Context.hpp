// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/ContextSettings.hpp>
#include <storm/engine/render/PhysicalDevice.hpp>

#include <storm/core/NonCopyable.hpp>
#include <storm/core/Pimpl.hpp>

namespace storm::engine {
	class ContextImpl;
	class Context : public core::NonCopyable {
		public:
			explicit Context(ContextSettings settings);
			~Context();
			
			Context(Context &&);
			Context &operator=(Context &&);
			
			PhysicalDevice bestPhysicalDevice();
			
			const ContextSettings &settings();
			
			IMPLEMENTATION(ContextImpl)
		private:
			core::Pimpl<ContextImpl> m_impl;
	};
}
