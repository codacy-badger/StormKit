file(GLOB OPENGL_SOURCES  src/opengl/*.cpp)
file(GLOB OPENGL_SHADER_SOURCES  src/opengl/shaders/*.glsl)
file(GLOB OPENGL_INCLUDES include/storm/engine/opengl/*.hpp include/storm/engine/opengl/*.tpp include/storm/engine/opengl/*.inl)

set_source_files_properties(
	${OPENGL_SHADER_SOURCES}
	PROPERTIES
	    LANGUAGE
		    GLSL
)

add_library(
	stormkit-engine-opengl
	SHARED
		${OPENGL_SOURCES}
		${OPENGL_INCLUDES}
)

target_link_libraries(
	stormkit-engine-opengl
	PUBLIC
	    StormKit::core
		StormKit::log
		StormKit::image
		StormKit::tools
		StormKit::window
		StormKit::sharedobject
		${FILESYSTEM_LIBRARIES}
	PRIVATE
	    OpenGL::OpenGL 
	    EGL::EGL
)

target_include_directories(
	stormkit-engine-opengl
	PUBLIC
	    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(
	stormkit-engine-opengl
	PUBLIC
	    STORM_GLM_CONVERSION
		GLM_ENABLE_EXPERIMENTAL
		GLM_FORCE_RADIANS
		$<$<BOOL:STORM_ENABLE_DEBUG_LOGGING>:DEBUG_LOGGING>
		$<$<BOOL:STORM_ENABLE_DEBUG_LOGGING>:STORM_ENABLE_DEBUG_LOGGING>
)

enable_cpp17(stormkit-engine-opengl)
storm_append_prefix_and_suffix(engine-opengl)
storm_export(Engine-OpenGL)