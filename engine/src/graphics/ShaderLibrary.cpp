// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <cstring>

#include <storm/engine/graphics/ShaderLibrary.hpp>

#include "Shaders.hpp"

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
ShaderLibrary::ShaderLibrary() {
	constexpr auto vert_size = std::size(FORWARD_RENDER_VERT_SRC) * sizeof(int);
	auto vert = std::vector<std::byte>{};
	vert.resize(vert_size);
	std::memcpy(
		std::data(vert),
		std::data(FORWARD_RENDER_VERT_SRC),
		vert_size
	);

	constexpr auto frag_size = std::size(FORWARD_RENDER_FRAG_SRC) * sizeof(int);
	auto frag = std::vector<std::byte>{};
	frag.resize(frag_size);
	std::memcpy(
		std::data(frag),
		std::data(FORWARD_RENDER_FRAG_SRC),
		frag_size
	);

	m_sources.emplace(FORWARD_RENDER_VERT, vert);
	m_sources.emplace(FORWARD_RENDER_FRAG, frag);
}

/////////////////////////////////////
/////////////////////////////////////
ShaderLibrary::~ShaderLibrary() = default;

/////////////////////////////////////
/////////////////////////////////////
ShaderLibrary::ShaderLibrary(ShaderLibrary &&) = default;

/////////////////////////////////////
/////////////////////////////////////
ShaderLibrary &ShaderLibrary::operator=(ShaderLibrary &&) = default;
