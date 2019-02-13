#pragma once
#include <cstdint>
#include <storm/core/Assert.hpp>
#include <storm/core/Filesystem.hpp>
#include <storm/core/Memory.hpp>
#include <storm/core/Platform.hpp>
#include <vector>

// based on https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-on-write
namespace storm::image {
	namespace private_ {
		struct ImageData;
	}
	class STORM_PUBLIC Image {
	public:
		SUR_Object(Image)

		    using size_type = std::size_t;
		enum class Format { RGB32, ARGB32, RGB16 };
		enum class Codec { AUTODETECT, JPEG, PNG, TARGA, PPM, UNKNOW };
		enum class CodecArgs {
			BINARY,
			ASCII // for ppm
		};

		struct Size {
			size_type width;
			size_type height;
		};

		explicit Image();
		explicit Image(const _std::filesystem::path &filepath,
		    Codec                                    codec = Codec::AUTODETECT);
		explicit Image(std::uint8_t *data, size_type size,
		    Codec codec = Codec::AUTODETECT);
		~Image() = default;

		void loadFromFile(const _std::filesystem::path &filepath,
		    Codec codec = Codec::AUTODETECT);
		void loadFromMemory(std::uint8_t *data, size_type size,
		    Codec codec = Codec::AUTODETECT);
		void create(std::uint32_t width, std::uint32_t height,
		    std::uint32_t channel_count = 3);

		std::uint8_t *      operator[](size_type i);
		const std::uint8_t *operator[](size_type i) const noexcept;
		std::uint8_t *      operator()(size_type x, size_type y);
		const std::uint8_t *operator()(size_type x, size_type y) const noexcept;

		Size         size() const noexcept;
		std::uint8_t channels() const noexcept;

		const size_type     byteNumbers() const noexcept;
		const std::uint8_t *data() const noexcept;
		std::uint8_t *      data();

		static Image scale(const Image &src, const Size &newSize);
		static Image flipX(const Image &src);
		static Image flipY(const Image &src);
		static Image rotate90(const Image &src);
		static Image rotate180(const Image &src);
		static Image rotate270(const Image &src);

		void saveToFile(const _std::filesystem::path &filename,
		    Codec                                     codec = Codec::AUTODETECT,
		    CodecArgs                                 args = CodecArgs::BINARY);

		void addChannels(std::uint8_t count = 1);

	private:
		void detach();

		std::shared_ptr<private_::ImageData> m_data;
	};
}
