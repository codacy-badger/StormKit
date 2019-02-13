#include "ImageLoader.hpp"

#include <fstream>
#include <storm/core/Strings.hpp>

extern "C" {
#include <jerror.h>
#include <jpeglib.h>
#include <png.h>

void jpeg_error_callback(jpeg_common_struct *st) {
	// throw std::runtime_error(storm::core::format("Jpegl lib error:"));
}
}

using namespace storm::image;
using namespace private_;

/*void _png_readFromStream(png_structp png_ptr, std::uint8_t* data, std::size_t
length) { std::ifstream* stream =
reinterpret_cast<std::ifstream*>(png_get_io_ptr(png_ptr));
    stream->read(reinterpret_cast<char*>(data), length);
}*/

ImageData::SPtr ImageLoader::loadPng(const _std::filesystem::path &filename) {
	ImageData::SPtr data = std::make_shared<ImageData>();

	{
#ifdef STORM_OS_WINDOWS
		std::FILE *file = nullptr;
		fopen_s(&file, filename.string().c_str(), "rb");
#else
		std::FILE *file = std::fopen(filename.string().c_str(), "rb");
#endif
		if (!file)
			throw std::runtime_error(storm::core::format(
			    "Failed to open file %{1}", filename.string()));

		unsigned char header[8];
		fread(header, 1, 8, file);
		if (png_sig_cmp(header, 0, 8))
			throw std::runtime_error(storm::core::format(
			    "[libpng] Failed to read header of %{1}", filename.string()));

		auto png_ptr = png_create_read_struct(
		    PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!png_ptr)
			throw std::runtime_error(storm::core::format(
			    "[libpng] Failed to init (png_create_read_struct) %{1}",
			    filename.string()));

		auto info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
			throw std::runtime_error(storm::core::format(
			    "[libpng] Failed to init (png_create_info_struct) %f",
			    filename.string()));

		if (setjmp(png_jmpbuf(png_ptr)))
			throw std::runtime_error(storm::core::format(
			    "[libpng] Failed to init io %{1}", filename.string()));

		png_init_io(png_ptr, file);
		png_set_sig_bytes(png_ptr, 8);
		// png_set_read_fn(png_ptr, &file, _png_readFromStream);

		png_read_info(png_ptr, info_ptr);

		png_set_interlace_handling(png_ptr);
		png_set_strip_16(png_ptr);
		png_set_packing(png_ptr);

		data->size.width  = png_get_image_width(png_ptr, info_ptr);
		data->size.height = png_get_image_height(png_ptr, info_ptr);
		data->channel     = png_get_channels(png_ptr, info_ptr);

		auto bit_depth  = png_get_bit_depth(png_ptr, info_ptr);
		auto color_type = png_get_color_type(png_ptr, info_ptr);

		switch (color_type) {
		case PNG_COLOR_TYPE_GRAY:
			png_set_expand_gray_1_2_4_to_8(png_ptr);
			data->channel = 1;
			break;

		case PNG_COLOR_TYPE_GRAY_ALPHA:
			png_set_expand_gray_1_2_4_to_8(png_ptr);
			data->channel = 2;
			break;

		case PNG_COLOR_TYPE_RGB:
			data->channel = 3;
			break;

		case PNG_COLOR_TYPE_RGB_ALPHA:
			data->channel = 4;
			break;
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(png_ptr);
			if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
				png_set_tRNS_to_alpha(png_ptr);
			png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
			data->channel = 4;
			break;

		default:

			break;
		}

		if (bit_depth == 16)
			png_set_strip_16(png_ptr);

		else if (bit_depth < 8)
			png_set_packing(png_ptr);

		png_read_update_info(png_ptr, info_ptr);

		if (setjmp(png_jmpbuf(png_ptr)))
			throw std::runtime_error(storm::core::format(
			    "[libpng] Failed to read image %{1}", filename.string()));

		data->data.resize(
		    data->size.width * data->size.height * data->channel, 0);
		std::vector<std::uint8_t *> row_pointers(data->size.height, 0);
		std::uint8_t *              buff_pos
		    = reinterpret_cast<std::uint8_t *>(data->data.data());

		for (size_t i = 0; i < data->size.height; ++i)
			row_pointers[i] = reinterpret_cast<png_bytep>(
			    buff_pos
			    + ((data->size.height - (i + 1)) * data->size.width
			        * data->channel));

		png_read_image(png_ptr, row_pointers.data());
		png_read_end(png_ptr, info_ptr);
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

		fclose(file);
	}

	return data;
}

ImageData::SPtr ImageLoader::loadPng(
    std::uint8_t *data, Image::size_type size) {
	ASSERT(false, "Not implemented yet !");

	return nullptr;
}

ImageData::SPtr ImageLoader::loadJpeg(const _std::filesystem::path &filename) {
	ImageData::SPtr data = std::make_shared<ImageData>();

	{
		struct jpeg_decompress_struct info;
		struct jpeg_error_mgr         error_mgr;

#ifdef STORM_OS_WINDOWS
		std::FILE *file = nullptr;
		fopen_s(&file, filename.string().c_str(), "rb");
#else
		std::FILE *file = std::fopen(filename.string().c_str(), "rb");
#endif
		if (!file)
			throw std::runtime_error(storm::core::format(
			    "Failed to open file %{1}", filename.string()));

		// init
		info.err             = jpeg_std_error(&error_mgr);
		error_mgr.error_exit = jpeg_error_callback;

		jpeg_create_decompress(&info);

		jpeg_stdio_src(&info, file);
		jpeg_read_header(&info, TRUE);

		jpeg_start_decompress(&info);

		data->size.width  = info.output_width;
		data->size.height = info.output_height;
		data->channel     = info.num_components;

		data->data.resize(data->size.width * data->size.height * data->channel);

		std::uint8_t *rowptr[1];
		while (info.output_scanline < info.output_height) {
			rowptr[0]
			    = data->data.data()
			      + data->size.width * data->channel * info.output_scanline;
			jpeg_read_scanlines(&info, rowptr, 1);
		}

		jpeg_finish_decompress(&info);

		fclose(file);
		file = nullptr;
	}

	return data;
}

ImageData::SPtr ImageLoader::loadJpeg(
    std::uint8_t *data, Image::size_type size) {
	ImageData::SPtr data_ = std::make_shared<ImageData>();

	{
		struct jpeg_decompress_struct info;
		struct jpeg_error_mgr         error_mgr;

		info.err             = jpeg_std_error(&error_mgr);
		error_mgr.error_exit = jpeg_error_callback;

		jpeg_create_decompress(&info);

		jpeg_mem_src(&info, data, static_cast<unsigned long>(size));

		jpeg_read_header(&info, TRUE);

		jpeg_start_decompress(&info);

		data_->size.width  = info.output_width;
		data_->size.height = info.output_height;
		data_->channel     = info.num_components;

		data_->data.resize(
		    data_->size.width * data_->size.height * data_->channel);

		std::uint8_t *rowptr[1];
		while (info.output_scanline < info.output_height) {
			rowptr[0]
			    = data_->data.data()
			      + data_->size.width * data_->channel * info.output_scanline;
			jpeg_read_scanlines(&info, rowptr, 1);
		}

		jpeg_finish_decompress(&info);
	}

	return data_;
}

ImageData::SPtr ImageLoader::loadTga(const _std::filesystem::path &filename) {
	ImageData::SPtr data = std::make_shared<ImageData>();

	{
		std::ifstream stream(filename.string(), std::ios::binary);

		struct {
			std::uint8_t magic[12];
			std::uint8_t header[6];
		} header;

		stream.read(reinterpret_cast<char *>(&header), sizeof(header));

		data->channel = 3;
		if (header.header[4] > 24)
			data->channel = 4;

		data->size.width  = header.header[1] * 256 + header.header[0];
		data->size.height = header.header[3] * 256 + header.header[2];

		const auto bytes       = header.header[4] / 8;
		const auto memory_size = data->size.width * data->size.height * bytes;

		if (header.magic[2] == 2) {
			data->data.resize(memory_size);

			stream.read(
			    reinterpret_cast<char *>(&(data->data[0])), memory_size);

			for (auto swap = 0; swap < memory_size; swap += bytes) {
				data->data[swap] ^= data->data[swap + 2] ^= data->data[swap]
				    ^= data->data[swap + 2];
			}
		} else if (header.magic[2] == 10) {
			auto pixel_count   = data->size.width * data->size.height;
			auto current_pixel = 0;
			auto current_byte  = 0;

			// Storage For 1 Pixel
			std::uint8_t *colorbuffer = new std::uint8_t[bytes];

			do {
				std::uint8_t chunk_header;
				stream.read(reinterpret_cast<char *>(&chunk_header),
				    sizeof(chunk_header));

				if (chunk_header < 128) {
					++chunk_header;
					for (auto i = 0; i < chunk_header; ++i) {
						stream.read(
						    reinterpret_cast<char *>(&colorbuffer), bytes);

						data->data[current_byte]     = colorbuffer[2];
						data->data[current_byte + 1] = colorbuffer[1];
						data->data[current_byte + 2] = colorbuffer[0];
						if (bytes == 4)
							data->data[current_byte + 3] = colorbuffer[3];

						current_byte += bytes;
						++current_pixel;
					}
				} else {
					chunk_header -= 127;

					stream.read(reinterpret_cast<char *>(&colorbuffer), bytes);

					for (auto i = 0; i < chunk_header; ++i) {
						data->data[current_byte]     = colorbuffer[2];
						data->data[current_byte + 1] = colorbuffer[1];
						data->data[current_byte + 2] = colorbuffer[0];
						if (bytes == 4)
							data->data[current_byte + 3] = colorbuffer[3];

						current_byte += bytes;
						++current_pixel;
					}
				}
			} while (current_pixel < pixel_count);
		}
	}
	return data;
}

ImageData::SPtr ImageLoader::loadTga(
    std::uint8_t *data, Image::size_type size) {
	ASSERT(false, "Not implemented yet !");

	return nullptr;
}

ImageData::SPtr ImageLoader::loadPPM(const _std::filesystem::path &filename) {
	ASSERT(false, "Not implemented yet !");

	return nullptr;
}

ImageData::SPtr ImageLoader::loadPPM(
    std::uint8_t *data, Image::size_type size) {
	ASSERT(false, "Not implemented yet !");

	return nullptr;
}

void ImageLoader::savePng(const _std::filesystem::path &filename,
    ImageData::SPtr &data, Image::CodecArgs args) {
	ASSERT(false, "Not implemented yet !");
}

void ImageLoader::saveJpeg(const _std::filesystem::path &filename,
    ImageData::SPtr &data, Image::CodecArgs args) {
	ASSERT(false, "Not implemented yet !");
}

void ImageLoader::saveTga(const _std::filesystem::path &filename,
    ImageData::SPtr &data, Image::CodecArgs args) {
	ASSERT(false, "Not implemented yet !");
}

void ImageLoader::savePPM(const _std::filesystem::path &filename,
    ImageData::SPtr &data, Image::CodecArgs args) {
	std::ofstream inp;

	if (args == Image::CodecArgs::BINARY) {
		inp.open(filename.string(), std::ios::out | std::ios::binary);
		if (inp.is_open()) {

			inp << "P6\n";
			inp << data->size.width;
			inp << " ";
			inp << data->size.height << "\n";
			inp << 255 << "\n";

			auto array = data->data;
			for (unsigned int i = 0; i < data->size.height; ++i) {
				for (unsigned int j = 0; j < data->size.width; ++j) {
					auto base = (i * data->size.width + j) * data->channel;

					auto r = array[base];
					auto g = array[base + 1];
					auto b = array[base + 2];

					inp.write(reinterpret_cast<char *>(&r), 1);
					inp.write(reinterpret_cast<char *>(&g), 1);
					inp.write(reinterpret_cast<char *>(&b), 1);
				}
			}
		}
	} else if (args == Image::CodecArgs::ASCII) {
		inp.open(filename.string(), std::ios::out);
		if (inp.is_open()) {
			inp << "P3\n";
			inp << data->size.width;
			inp << " ";
			inp << data->size.height << "\n";
			inp << 255 << "\n";

			auto array = data->data;
			for (unsigned int i = 0; i < data->size.height; ++i) {
				for (unsigned int j = 0; j < data->size.width; ++j) {
					auto base = (i * data->size.width + j) * data->channel;

					auto r = array[base];
					auto g = array[base + 1];
					auto b = array[base + 2];

					inp << int(r) << " ";
					inp << int(g) << " ";
					inp << int(b) << "   " << std::flush;
				}
				inp << "\n";
			}
		}
	}
	inp.close();
}
