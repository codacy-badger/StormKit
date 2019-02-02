#pragma once

#include <storm/core/NonMovable.hpp>
#include <storm/core/NonCopyable.hpp>

#include <storm/tools/ResourcesPool.hpp>

#include <storm/image/Image.hpp>

namespace storm::engine {
	struct STORM_PUBLIC ImagePool : storm::core::NonCopyable, storm::core::NonMovable  {
		tools::ResourcesPool<std::string, image::Image> pool;

		static ImagePool global_pool;
	};
}


