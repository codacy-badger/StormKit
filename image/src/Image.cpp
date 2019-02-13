#include "private/ImageData.hpp"
#include "private/ImageLoader.hpp"

#include <storm/core/Strings.hpp>
#include <storm/image/Image.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::image;

namespace storm::image {
	Image::Codec filenameToCodec(const _std::filesystem::path &filename) {
		const std::string ext = filename.extension().string();

		if (storm::core::to_lower(ext) == ".jpg"
		    || storm::core::to_lower(ext) == ".jpeg")
			return Image::Codec::JPEG;
		else if (storm::core::to_lower(ext) == ".png")
			return Image::Codec::PNG;
		else if (storm::core::to_lower(ext) == ".tga"
		         || storm::core::to_lower(ext) == ".targa")
			return Image::Codec::TARGA;
		else if (storm::core::to_lower(ext) == ".ppm")
			return Image::Codec::PPM;

		return Image::Codec::UNKNOW;
	}
}

/////////////////////////////////////
/////////////////////////////////////
Image::Image() : m_data(nullptr) {}

/////////////////////////////////////
/////////////////////////////////////
Image::Image(const _std::filesystem::path &filepath, Image::Codec codec)
    : Image() {
	loadFromFile(filepath, codec);
}

/////////////////////////////////////
/////////////////////////////////////
Image::Image(std::uint8_t *data, size_type size, Image::Codec codec) : Image() {
	loadFromMemory(data, size, codec);
}

/////////////////////////////////////
/////////////////////////////////////
void Image::loadFromFile(
    const _std::filesystem::path &filepath, Image::Codec codec) {
	detach();

	if (codec == Image::Codec::AUTODETECT)
		codec = filenameToCodec(filepath);

	ASSERT(codec != Image::Codec::UNKNOW, "Unknow image filetype");

	switch (codec) {
	case Image::Codec::JPEG:
		m_data = private_::ImageLoader::loadJpeg(filepath);
		break;
	case Image::Codec::PNG:
		m_data = private_::ImageLoader::loadPng(filepath);
		break;
	case Image::Codec::TARGA:
		m_data = private_::ImageLoader::loadTga(filepath);
		break;
	case Image::Codec::PPM:
		m_data = private_::ImageLoader::loadPPM(filepath);
		break;
	default:
		throw std::runtime_error("oops");
	}
}

/////////////////////////////////////
/////////////////////////////////////
void Image::loadFromMemory(
    std::uint8_t *data, size_type size, Image::Codec codec) {
	detach();

	ASSERT(codec != Image::Codec::AUTODETECT,
	    "Autodetection only for file based image loading");
	ASSERT(codec != Image::Codec::UNKNOW, "Unknow image filetype");

	switch (codec) {
	case Image::Codec::JPEG:
		m_data = private_::ImageLoader::loadJpeg(data, size);
		break;
	case Image::Codec::PNG:
		m_data = private_::ImageLoader::loadPng(data, size);
		break;
	case Image::Codec::TARGA:
		m_data = private_::ImageLoader::loadTga(data, size);
		break;
	case Image::Codec::PPM:
		m_data = private_::ImageLoader::loadPPM(data, size);
		break;
	default:
		throw std::runtime_error("oops");
	}
}

/////////////////////////////////////
/////////////////////////////////////
void Image::create(
    std::uint32_t width, std::uint32_t height, std::uint32_t channel_count) {
	m_data          = std::make_shared<private_::ImageData>();
	m_data->size    = {width, height};
	m_data->channel = channel_count;
	m_data->data.resize(width * height * m_data->channel);
}

/////////////////////////////////////
/////////////////////////////////////
std::uint8_t *Image::operator[](Image::size_type i) {
	detach();

	return &m_data->data[i];
}

/////////////////////////////////////
/////////////////////////////////////
const std::uint8_t *Image::operator[](Image::size_type i) const noexcept {
	return &m_data->data[i];
}

/////////////////////////////////////
/////////////////////////////////////
std::uint8_t *Image::operator()(Image::size_type x, Image::size_type y) {
	detach();

	std::uint8_t *pointer
	    = &m_data->data[(y * m_data->size.width + x) * m_data->channel];

	return pointer;
}

/////////////////////////////////////
/////////////////////////////////////
const std::uint8_t *Image::operator()(
    Image::size_type x, Image::size_type y) const noexcept {
	const std::uint8_t *pointer
	    = &m_data->data[(y * m_data->size.width + x) * m_data->channel];

	return pointer;
}

/////////////////////////////////////
/////////////////////////////////////
Image::Size Image::size() const noexcept {
	return (!m_data) ? Image::Size {0, 0} : m_data->size;
}

/////////////////////////////////////
/////////////////////////////////////
std::uint8_t Image::channels() const noexcept {
	return (!m_data) ? 0 : m_data->channel;
}

const Image::size_type Image::byteNumbers() const noexcept {
	return (!m_data) ? 0 : m_data->data.size();
}

/////////////////////////////////////
/////////////////////////////////////
const std::uint8_t *Image::data() const noexcept {
	return (!m_data) ? nullptr : m_data->data.data();
}

/////////////////////////////////////
/////////////////////////////////////
std::uint8_t *Image::data() {
	if (!m_data)
		return nullptr;

	detach();

	return m_data->data.data();
}

/////////////////////////////////////
/////////////////////////////////////
Image Image::scale(const Image &src, const Image::Size &newSize) { return src; }

/////////////////////////////////////
/////////////////////////////////////
Image Image::flipX(const Image &src) {
	Image out = src;
	out.detach();

	for (auto x = 0; x < out.size().width; ++x)
		for (auto y = 0; y < out.size().height; ++y)
			for (auto k = 0; k < src.channels(); ++k)
				out.data()[(x + y * out.size().width) * src.channels() + k]
				    = src.data()[(x + (out.size().height) * out.size().width - 1
				                     - x)
				                     * src.channels()
				                 + k];

	return out;
}

/////////////////////////////////////
/////////////////////////////////////
Image Image::flipY(const Image &src) {
	Image out = src;
	out.detach();
	std::memset(out.data(), 0, out.size().width * out.size().height);

	for (auto x = 0; x < out.size().width; ++x)
		for (auto y = 0; y < out.size().height; ++y)
			for (auto k = 0; k < src.channels(); ++k)
				out.data()[(x + y * out.size().width) * src.channels() + k]
				    = src.data()
				          [(x + (out.size().height - 1 - y) * out.size().width)
				                  * src.channels()
				              + k];

	return out;
}

/////////////////////////////////////
/////////////////////////////////////
Image Image::rotate90(const Image &src) { return src; }

/////////////////////////////////////
/////////////////////////////////////
Image Image::rotate180(const Image &src) { return src; }

/////////////////////////////////////
/////////////////////////////////////
Image Image::rotate270(const Image &src) { return src; }

/////////////////////////////////////
/////////////////////////////////////
void Image::saveToFile(
    const _std::filesystem::path &filename, Codec codec, CodecArgs args) {
	if (codec == Image::Codec::AUTODETECT)
		codec = filenameToCodec(filename);

	switch (codec) {
	case Image::Codec::JPEG:
		private_::ImageLoader::saveJpeg(filename, m_data, args);
		break;
	case Image::Codec::PNG:
		private_::ImageLoader::savePng(filename, m_data, args);
		break;
	case Image::Codec::TARGA:
		private_::ImageLoader::saveTga(filename, m_data, args);
		break;
	case Image::Codec::PPM:
		private_::ImageLoader::savePPM(filename, m_data, args);
		break;
	default:
		throw std::runtime_error("oops");
	}
}

void Image::addChannels(std::uint8_t count) {
	detach();

	const auto pixel_count = m_data->size.width * m_data->size.height;

	auto image_data = std::vector<std::uint8_t> {};
	image_data.reserve(pixel_count * (m_data->channel + count));

	for (auto i = 0; i < pixel_count; ++i) {
		std::copy_n(std::begin(m_data->data) + (i * m_data->channel),
		    m_data->channel, std::back_inserter(image_data));

		for (auto j = 0; j < count; ++j)
			image_data.emplace_back(0);
	}

	m_data->channel += count;
	m_data->data = std::move(image_data);
}

/////////////////////////////////////
/////////////////////////////////////
void Image::detach() {
	if (m_data && !m_data.unique()) {
		private_::ImageData *tmp = m_data.get();
		m_data                   = std::make_shared<private_::ImageData>(*tmp);
	}
}
