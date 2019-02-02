project(StormKitLog VERSION 0.2.0 LANGUAGES C CXX)

add_library(
	stormkit-log
	SHARED
	src/LogOutput.cpp
	src/ConsoleOutput.cpp
	src/FileOutput.cpp
	include/storm/log/LogOutput.hpp
	include/storm/log/ConsoleOutput.hpp
	include/storm/log/FileOutput.hpp
)

add_library(StormKit::log ALIAS stormkit-log)

target_include_directories(
	stormkit-log
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


message(STATUS "${FILESYSTEM_LIBRARIES}")

target_link_libraries(
	stormkit-log
	PUBLIC
		StormKit::core
		${FILESYSTEM_LIBRARIES}
)

target_compile_definitions(
	stormkit-log
	PUBLIC
		BOOST_SYSTEM_NO_DEPRECATED
)

enable_cpp17(stormkit-log)
storm_append_prefix_and_suffix(log)
storm_export(Log)

