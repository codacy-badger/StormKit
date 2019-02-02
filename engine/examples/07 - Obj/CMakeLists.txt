project(Obj VERSION 0.2.0 LANGUAGES C CXX)

add_executable(
	obj
	src/main.cpp
	src/tiny_obj_loader.h
)

target_link_libraries(
	obj
	PUBLIC
	    StormKit::core
		StormKit::window
		StormKit::log
		StormKit::engine
		Threads::Threads
)

target_compile_definitions(
	obj
	PUBLIC
	    GLM_FORCE_RADIANS
		GLM_FORCE_DEPTH_ZERO_TO_ONE
		GLM_FORCE_LEFT_HANDED
)

configure_file(shaders/obj_spv.vert ${CMAKE_CURRENT_BINARY_DIR}/shaders/obj_spv.vert COPYONLY)
configure_file(shaders/obj_spv.frag ${CMAKE_CURRENT_BINARY_DIR}/shaders/obj_spv.frag COPYONLY)
file(COPY models DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

enable_cpp17(obj)

