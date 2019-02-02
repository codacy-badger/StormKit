project(StormKitImage VERSION 0.2.0 LANGUAGES C CXX)

add_library(
	stormkit-image
	SHARED
	src/Image.cpp
	src/private/ImageData.hpp
	src/private/ImageLoader.cpp
	src/private/ImageLoader.hpp
	include/storm/image/Image.hpp
)

add_library(StormKit::image ALIAS stormkit-image)

target_link_libraries(
	stormkit-image
	PUBLIC
		StormKit::core
		StormKit::log
		PNG::PNG
		TURBOJPEG::TURBOJPEG
		${FILESYSTEM_LIBRARIES}
)

target_include_directories(
	stormkit-image
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

list(APPEND FIND_DEPENDENCY_FILES FindTURBOJPEG.cmake)

enable_cpp17(stormkit-image)
storm_append_prefix_and_suffix(image)
storm_export(Image FIND_DEPENDENCY_FILES)


