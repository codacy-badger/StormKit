project(StormKitEntities VERSION 0.2.0 LANGUAGES C CXX)

add_library(
	stormkit-entities
	SHARED
	src/EntityManager.cpp
	src/System.cpp
	include/storm/entities/Component.hpp
	include/storm/entities/Entity.hpp
	include/storm/entities/EntityManager.hpp
	include/storm/entities/EntityManager.tpp
	include/storm/entities/System.hpp
)

add_library(StormKit::entities ALIAS stormkit-entities)

target_include_directories(
	stormkit-entities
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
	stormkit-entities
	PUBLIC
		StormKit::core
)

enable_cpp17(stormkit-entities)
storm_append_prefix_and_suffix(entities)
storm_export(Entities)

