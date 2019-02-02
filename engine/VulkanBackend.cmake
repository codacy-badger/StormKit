file(GLOB VULKAN_SOURCES  src/vulkan/*.cpp)
file(GLOB VULKAN_SHADER_SOURCES  src/vulkan/shaders/*.glsl)
file(GLOB VULKAN_INCLUDES include/storm/engine/vulkan/*.hpp include/storm/engine/vulkan/*.tpp include/storm/engine/vulkan/*.inl)

set_source_files_properties(
	${VULKAN_SHADER_SOURCES}
	PROPERTIES
	    LANGUAGE
		    GLSL
)

if(STORM_OS_WIN32)
	set(VK_COMPILE_DEFINITION "VK_USE_PLATFORM_WIN32_KHR")
elseif(STORM_OS_MACOS)
	set(VK_COMPILE_DEFINITION "VK_USE_PLATFORM_MACOS_MVK")
elseif(STORM_OS_LINUX)
	if(STORM_ENABLE_WINDOW_BACKEND_X11)
		set(VK_COMPILE_DEFINITION "VK_USE_PLATFORM_XCB_KHR")
	elseif(STORM_ENABLE_WINDOW_BACKEND_WAYLAND)
		set(VK_COMPILE_DEFINITION "VK_USE_PLATFORM_WAYLAND_KHR")
	endif()
endif()

add_library(
	stormkit-engine-vulkan
	SHARED
		${VULKAN_SOURCES}
		${VULKAN_INCLUDES}
)

target_link_libraries(
	stormkit-engine-vulkan
	PUBLIC
	    StormKit::core
		StormKit::log
		StormKit::image
		StormKit::tools
		StormKit::window
		StormKit::sharedobject
		StormKit::engine
		${FILESYSTEM_LIBRARIES}
	PRIVATE
	    Vulkan::Vulkan
)

target_include_directories(
	stormkit-engine-vulkan
	PUBLIC
	    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(
	stormkit-engine-vulkan
	PUBLIC
	    STORM_GLM_CONVERSION
		GLM_ENABLE_EXPERIMENTAL
		GLM_FORCE_RADIANS
		GLM_FORCE_DEPTH_ZERO_TO_ONE
		$<$<BOOL:STORM_ENABLE_DEBUG_LOGGING>:DEBUG_LOGGING>
		$<$<BOOL:STORM_ENABLE_DEBUG_LOGGING>:STORM_ENABLE_DEBUG_LOGGING>
	PRIVATE
	    ${VK_COMPILE_DEFINITION}
)

enable_cpp17(stormkit-engine-vulkan)
storm_append_prefix_and_suffix(engine-vulkan)
storm_export(Engine-Vulkan)
