#pragma once
#include "ImageData.hpp"

#include <storm/core/NonInstanciable.hpp>

namespace storm::image::private_ {
    class ImageLoader : public storm::core::NonInstanciable {
    public:
        static ImageData::SPtr loadPng(const _std::filesystem::path &filename);
        static ImageData::SPtr loadPng(std::uint8_t *data, Image::size_type size);
        static ImageData::SPtr loadJpeg(const _std::filesystem::path &filename);
        static ImageData::SPtr loadJpeg(std::uint8_t *data, Image::size_type size);
        static ImageData::SPtr loadTga(const _std::filesystem::path &filename);
        static ImageData::SPtr loadTga(std::uint8_t *data, Image::size_type size);
        static ImageData::SPtr loadPPM(const _std::filesystem::path &filename);
        static ImageData::SPtr loadPPM(std::uint8_t *data, Image::size_type size);

        static void savePng(
          const _std::filesystem::path &filename,
          ImageData::SPtr &data,
          Image::CodecArgs args);
        static void saveJpeg(
          const _std::filesystem::path &filename,
          ImageData::SPtr &data,
          Image::CodecArgs args);
        static void saveTga(
          const _std::filesystem::path &filename,
          ImageData::SPtr &data,
          Image::CodecArgs args);
        static void savePPM(
          const _std::filesystem::path &filename,
          ImageData::SPtr &data,
          Image::CodecArgs args);
    };
} // namespace storm::image::private_
