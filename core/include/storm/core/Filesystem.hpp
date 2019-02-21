#pragma once

#include <storm/core/Configure.hpp>
#include <storm/core/Platform.hpp>

//#if defined(HAVE_FILESYSTEM)
#if __has_include(<filesystem>)
#    include <filesystem>
namespace _std {
#    if defined(__has_cpp_attribute) && defined(__cpp_lib_filesystem)
    namespace filesystem = std::filesystem;
#    else
    namespace filesystem = std::experimental::filesystem;
#    endif
} // namespace _std
#elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
namespace _std {
#    if defined(__has_cpp_attribute) && defined(__cpp_lib_filesystem)
    namespace filesystem = std::filesystem;
#    else
    namespace filesystem = std::experimental::filesystem;
#    endif
} // namespace _std
#elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
namespace _std {
    namespace filesystem = boost::filesystem;
}
#endif
