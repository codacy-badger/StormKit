// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/graphics/DrawableCache.hpp>
#include <storm/engine/graphics/Drawable.hpp>

#include <storm/core/Assert.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
DrawableCache::DrawableCache() = default;

/////////////////////////////////////
/////////////////////////////////////
DrawableCache::~DrawableCache() = default;

/////////////////////////////////////
/////////////////////////////////////
DrawableCache::DrawableCache(DrawableCache&&) = default;

/////////////////////////////////////
/////////////////////////////////////
DrawableCache &DrawableCache::operator=(DrawableCache&&) = default;

/////////////////////////////////////
/////////////////////////////////////
std::uint64_t DrawableCache::addDrawable(const Drawable &drawable) {
	m_opaque_drawables.emplace_back(drawable);
	
	auto id = std::size(m_opaque_drawables);
	
	return id;
}

/////////////////////////////////////
/////////////////////////////////////
std::uint64_t DrawableCache::addDrawable(Drawable &&drawable) {
	m_opaque_drawables.emplace_back(std::move(drawable));
	
	auto id = std::size(m_opaque_drawables);
	
	return id;
}

/////////////////////////////////////
/////////////////////////////////////
void DrawableCache::removeDrawable(std::uint64_t key) {
	ASSERT(key > std::size(m_opaque_drawables), "");
	
	m_opaque_drawables.erase(std::begin(m_opaque_drawables) + key);
}

/////////////////////////////////////
/////////////////////////////////////
Drawable &DrawableCache::drawable(std::uint64_t key) {
	ASSERT(key > std::size(m_opaque_drawables), "");
			
	return m_opaque_drawables[key];
}

/////////////////////////////////////
/////////////////////////////////////
const Drawable &DrawableCache::drawable(std::uint64_t key) const {
	ASSERT(key > std::size(m_opaque_drawables), "");
			
	return m_opaque_drawables[key];
}
		
