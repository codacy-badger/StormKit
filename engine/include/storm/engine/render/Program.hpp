// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/Pimpl.hpp>
#include <storm/core/Memory.hpp>

#include <storm/engine/render/Utils.hpp>

#include <vector>

namespace storm::engine {
	class ProgramImpl;
	class Program : public core::NonCopyable {
		public:
			Unique_Object(Program)
			Ref_Object(Program)

			explicit Program(const Device &device);
			~Program();

			Program(Program &&);
			Program &operator=(Program &&);

			void addShaderModule(const Shader &module);
			const std::vector<const Shader*> &shaderModules() const noexcept;

			void link();

			IMPLEMENTATION(ProgramImpl)
		private:
			core::Pimpl<ProgramImpl> m_impl;
	};
}
