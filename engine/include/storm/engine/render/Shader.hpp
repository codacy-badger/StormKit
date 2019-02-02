// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <storm/core/Filesystem.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/Memory.hpp>
#include <storm/core/Pimpl.hpp>

#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/Utils.hpp>

namespace storm::engine {
	class ShaderImpl;
	class Shader : public core::NonCopyable {
		public:
			Unique_Object(Shader)

			enum class Stage {
				VERTEX,
				FRAGMENT,
				GEOMETRY,
				COMPUTE,
				TESSELATION_CONTROL,
				TESSELATION_EVALUATION,
				SIZE
			};

			enum class DefaultShader {
				DEFAULT_VERT_2D,
				DEFAULT_FRAG_2D
			};

			explicit Shader(const Device &device, DefaultShader shader);
			explicit Shader(const Device &device, Stage stage, const _std::filesystem::path &path);
			explicit Shader(const Device &device, Stage stage, const std::byte* data, std::size_t size);
			~Shader();

			Shader(Shader &&);

			Stage stage() const noexcept;
			const std::vector<std::byte> &source() const noexcept;

			IMPLEMENTATION(ShaderImpl)
		private:
			core::Pimpl<ShaderImpl> m_impl;
	};
}
