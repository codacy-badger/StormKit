project(UniformBuffer VERSION 0.2.0 LANGUAGES C CXX)

add_executable(
	uniformbuffer
	src/main.cpp
)

target_link_libraries(
	uniformbuffer
	PUBLIC
	    StormKit::core
		StormKit::window
		StormKit::log
		StormKit::engine
		Threads::Threads
)

target_compile_definitions(
	uniformbuffer
	PUBLIC
	    GLM_FORCE_RADIANS
		GLM_FORCE_DEPTH_ZERO_TO_ONE
		GLM_FORCE_LEFT_HANDED
)

configure_file(shaders/uniformbuffer_spv.vert ${CMAKE_CURRENT_BINARY_DIR}/shaders/uniformbuffer_spv.vert COPYONLY)
configure_file(shaders/uniformbuffer_spv.frag ${CMAKE_CURRENT_BINARY_DIR}/shaders/uniformbuffer_spv.frag COPYONLY)

enable_cpp17(uniformbuffer)

