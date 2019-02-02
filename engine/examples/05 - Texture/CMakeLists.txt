project(Texture VERSION 0.2.0 LANGUAGES C CXX)

add_executable(
	texture
	src/main.cpp
)

target_link_libraries(
	texture
	PUBLIC
	    StormKit::core
		StormKit::window
		StormKit::log
		StormKit::engine
		StormKit::image
		Threads::Threads
)

target_compile_definitions(
	texture
	PUBLIC
	    GLM_FORCE_RADIANS
		GLM_FORCE_DEPTH_ZERO_TO_ONE
		GLM_FORCE_LEFT_HANDED
)

configure_file(shaders/texture_spv.vert ${CMAKE_CURRENT_BINARY_DIR}/shaders/texture_spv.vert COPYONLY)
configure_file(shaders/texture_spv.frag ${CMAKE_CURRENT_BINARY_DIR}/shaders/texture_spv.frag COPYONLY)
configure_file(textures/texture.png ${CMAKE_CURRENT_BINARY_DIR}/textures/texture.png COPYONLY)

enable_cpp17(texture)

