// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>

#include <storm/core/NonCopyable.hpp>

namespace storm::engine {
	class ShaderLibrary : public core::NonCopyable {
		public:
			static constexpr const auto FORWARD_RENDER_FRAG = "FORWARD_RENDER_FRAG";
			static constexpr const auto FORWARD_RENDER_VERT = "FORWARD_RENDER_VERT";

			explicit ShaderLibrary();
			~ShaderLibrary();

			ShaderLibrary(ShaderLibrary &&);
			ShaderLibrary &operator=(ShaderLibrary &&);

			inline const std::vector<std::byte> &getSource(std::string name) const noexcept;
		private:
			std::unordered_map<std::string, std::vector<std::byte>> m_sources;
	};
}

#include "ShaderLibrary.inl"
