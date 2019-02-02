project(StormKitEventbus VERSION 0.2.0 LANGUAGES C CXX)

add_library(stormkit-eventbus INTERFACE)
add_library(StormKit::eventbus ALIAS stormkit-eventbus)

add_custom_target(stormkit-eventbus_parseproject
	ALL SOURCES
	include/storm/eventbus/EventBus.hpp
	include/storm/eventbus/EventBus.tpp
)

target_include_directories(
	stormkit-eventbus
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)
target_compile_definitions(
	stormkit-eventbus

	INTERFACE LIBRARY_HEADER_ONLY
	INTERFACE STORM_BUILD
)

storm_export(Eventbus)
