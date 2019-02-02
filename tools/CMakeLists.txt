project(StormKitTools VERSION 0.2.0 LANGUAGES C CXX)

add_library(
	stormkit-tools
	SHARED
	    src/Observer.cpp
		src/State.cpp
		src/StateManager.cpp
		src/Tree.cpp
		src/TreeNode.cpp
		include/storm/tools/Observer.hpp
		include/storm/tools/ResourcesPool.hpp
		include/storm/tools/State.hpp
		include/storm/tools/StateManager.hpp
		include/storm/tools/Subject.hpp
		include/storm/tools/Subject.tpp
		include/storm/tools/Tree.hpp
		include/storm/tools/TreeNode.hpp
)

add_library(StormKit::tools ALIAS stormkit-tools)

target_link_libraries(
	stormkit-tools
	PUBLIC
	    StormKit::core
)

target_include_directories(
	stormkit-tools
	PUBLIC
	    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

enable_cpp17(stormkit-tools)
storm_append_prefix_and_suffix(tools)
storm_export(Tools)

