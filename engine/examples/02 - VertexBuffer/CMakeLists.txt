project(VertexBuffer VERSION 0.2.0 LANGUAGES C CXX)

add_executable(
	vertexbuffer
	src/main.cpp
)

target_link_libraries(
	vertexbuffer
	PUBLIC
	    StormKit::core
		StormKit::window
		StormKit::log
		StormKit::engine
		Threads::Threads
)

target_compile_definitions(
	vertexbuffer
	PUBLIC
	    GLM_FORCE_RADIANS
		GLM_FORCE_DEPTH_ZERO_TO_ONE
		GLM_FORCE_LEFT_HANDED
)

configure_file(shaders/vertexbuffer_spv.vert ${CMAKE_CURRENT_BINARY_DIR}/shaders/vertexbuffer_spv.vert COPYONLY)
configure_file(shaders/vertexbuffer_spv.frag ${CMAKE_CURRENT_BINARY_DIR}/shaders/vertexbuffer_spv.frag COPYONLY)

enable_cpp17(vertexbuffer)

