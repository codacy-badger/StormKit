#pragma once

#include <cstdint>

namespace storm::window {
	struct VideoSettings {
		struct Size {
			union {
				std::uint16_t width;
				std::uint16_t w;
			};
			union {
				std::uint16_t height;
				std::uint16_t h;
			};
		} size;
		union {
			std::uint8_t bitsPerPixel;
			std::uint8_t bpp;
		};
		union {
			std::uint16_t dotsPerInch;
			std::uint16_t dpi;
		};
	};
}
