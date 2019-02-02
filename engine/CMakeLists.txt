project(StormKitEngine VERSION 0.2.0 LANGUAGES C CXX)

file(GLOB RENDER_SOURCES  src/render/*.cpp)
file(GLOB RENDER_INCLUDES include/storm/engine/render/*.hpp include/storm/engine/render/*.tpp include/storm/engine/render/*.inl)
file(GLOB VULKAN_SOURCES  src/vulkan/*.cpp)
file(GLOB VULKAN_SHADER_SOURCES  src/vulkan/shaders/*.glsl)
file(GLOB VULKAN_INCLUDES include/storm/engine/vulkan/*.hpp include/storm/engine/vulkan/*.tpp include/storm/engine/vulkan/*.inl)

file(GLOB GRAPHICS_SOURCES  src/graphics/*.cpp src/graphics/*.hpp)
file(GLOB GRAPHICS_INCLUDES include/storm/engine/graphics/*.hpp include/storm/engine/graphics/*.tpp include/storm/engine/graphics/*.inl)

file(GLOB SCENEGRAPH_SOURCES  src/scenegraph/*.cpp src/scenegraph/private/*.cpp src/scenegraph/private/*.hpp)
file(GLOB SCENEGRAPH_INCLUDES include/storm/engine/scenegraph/*.hpp include/storm/engine/scenegraph/*.tpp include/storm/engine/scenegraph/*.inl)

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
	stormkit-engine
	SHARED
		${RENDER_SOURCES}
		${RENDER_INCLUDES}
		${VULKAN_SOURCES}
		${VULKAN_INCLUDES}
		${GRAPHICS_SOURCES}
		${GRAPHICS_INCLUDES}
		#${SCENEGRAPH_SOURCES}
		#${SCENEGRAPH_INCLUDES}
)

add_library(StormKit::engine ALIAS stormkit-engine)

target_link_libraries(
	stormkit-engine
	PUBLIC
	    StormKit::core
		StormKit::log
		StormKit::image
		StormKit::tools
		StormKit::window
		StormKit::sharedobject
		${FILESYSTEM_LIBRARIES}
		spirv-cross-cpp
	PRIVATE
	    Vulkan::Vulkan
	   # ASSIMP::ASSIMP
)

target_include_directories(
	stormkit-engine
	PUBLIC
	    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(
	stormkit-engine
	PUBLIC
	    STORM_GLM_CONVERSION
		GLM_ENABLE_EXPERIMENTAL
		GLM_FORCE_RADIANS
		GLM_FORCE_DEPTH_ZERO_TO_ONE
		GLM_FORCE_LEFT_HANDED
		GLM_FORCE_CXX14
		${VK_COMPILE_DEFINITION}
		$<$<BOOL:STORM_ENABLE_ENGINE_VULKAN_BACKEND>:STORM_ENABLE_ENGINE_VULKAN_BACKEND>
		$<$<BOOL:STORM_ENABLE_ENGINE_OPENGL_BACKEND>:STORM_ENABLE_ENGINE_OPENGL_BACKEND>
		$<$<BOOL:STORM_ENABLE_DEBUG_LOGGING>:DEBUG_LOGGING>
		$<$<BOOL:STORM_ENABLE_DEBUG_LOGGING>:STORM_ENABLE_DEBUG_LOGGING>
)

enable_cpp17(stormkit-engine)
storm_append_prefix_and_suffix(engine)
storm_export(Engine)

if(STORM_ENABLE_EXAMPLES)
	add_subdirectory("examples/01 - Triangle")
	add_subdirectory("examples/02 - VertexBuffer")
	add_subdirectory("examples/03 - IndexBuffer")
	add_subdirectory("examples/04 - UniformBuffer")
	add_subdirectory("examples/05 - Texture")
	add_subdirectory("examples/06 - DepthBuffer")
	add_subdirectory("examples/07 - Obj")
endif()




