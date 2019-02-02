#pragma once
#include <storm/core/Memory.hpp>
#include <storm/image/Image.hpp>

namespace storm::image::private_ {
	struct ImageData {
		Shared_Object_Wm(ImageData)
		Unique_Object_Wm(ImageData)

		Image::Size size     = {0, 0};
		std::uint8_t channel = 0;
		std::vector<std::uint8_t> data;
	};
}
