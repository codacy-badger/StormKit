// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once 

#include <storm/core/NonCopyable.hpp>
#include <storm/core/Memory.hpp>

#include <storm/engine/graphics/Hashes.hpp>

#include <unordered_map>
#include <vector>

namespace storm::engine {
	class DrawableCache : public core::NonCopyable {
		public:
			SUR_Object(DrawableCache)
			
			explicit DrawableCache();
			~DrawableCache();
			
			DrawableCache(DrawableCache&&);
			DrawableCache &operator=(DrawableCache&&);
			
			std::uint64_t addDrawable(const Drawable &drawable);
			std::uint64_t addDrawable(Drawable &&drawable);
			
			void removeDrawable(std::uint64_t key);
			
			Drawable &drawable(std::uint64_t key);
			const Drawable &drawable(std::uint64_t key) const;
			
			inline const std::vector<Drawable> drawables() const noexcept { return m_opaque_drawables; }
		private:
			std::vector<Drawable> m_opaque_drawables;
			
	};
}
