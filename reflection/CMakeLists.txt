project(StormKitReflection VERSION 0.2.0 LANGUAGES C CXX)

add_library(stormkit-reflection INTERFACE)
add_library(StormKit::reflection ALIAS stormkit-reflection)

add_custom_target(stormkit-reflection_parseproject
	ALL SOURCES
	include/storm/reflection/Exceptions.hpp
	include/storm/reflection/Function.hpp
	include/storm/reflection/FunctionBase.hpp
	include/storm/reflection/ReflectableManager.hpp
	include/storm/reflection/Reflection.hpp
)

target_include_directories(
	stormkit-reflection
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

target_compile_definitions(
	stormkit-reflection

	INTERFACE LIBRARY_HEADER_ONLY
	INTERFACE STORM_BUILD
)

storm_export(Reflection)
