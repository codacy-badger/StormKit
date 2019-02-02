project(HelloWindow VERSION 0.2.0 LANGUAGES C CXX)

add_executable(
	hello_window
	src/main.cpp
)

target_link_libraries(
	hello_window
	PUBLIC
	    StormKit::core
		StormKit::window
		StormKit::log
		Threads::Threads
)

enable_cpp17(hello_window)

