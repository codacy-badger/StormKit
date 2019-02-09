project(StormKitCore VERSION 0.2.0 LANGUAGES C CXX)

add_library(stormkit-core INTERFACE)
add_library(StormKit::core ALIAS stormkit-core)

add_custom_target(stormkit-core_parseproject
	ALL SOURCES
	include/storm/core/App.hpp
	include/storm/core/Assert.hpp
	include/storm/core/CRTP.hpp
	include/storm/core/Filesystem.hpp
	include/storm/core/Flags.hpp
	include/storm/core/Hash.hpp
	include/storm/core/Iterator.hpp
	include/storm/core/Memory.hpp
	include/storm/core/NamedType.hpp
	include/storm/core/NonCopyable.hpp
	include/storm/core/NonInstanciable.hpp
	include/storm/core/NonMovable.hpp
	include/storm/core/Numerics.hpp
	include/storm/core/Platform.hpp
	include/storm/core/Singleton.hpp
	include/storm/core/Strings.hpp
	include/storm/core/Pimpl.hpp
	include/storm/core/Pimpl.tpp
	include/storm/core/Timer.hpp
	include/storm/core/Timer.tpp
	include/storm/core/Types.hpp
	include/storm/core/TypeTraits.hpp
	include/storm/core/private/NamedTypeImpl.hpp
	include/storm/core/private/UnderlyingFunctionalities.hpp
	include/storm/core/private/incbin.h
	${CMAKE_CURRENT_BINARY_DIR}/generated/include/storm/core/Configure.hpp
)

message(${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(
	stormkit-core
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated/include/>
		$<INSTALL_INTERFACE:include>
)
target_compile_definitions(
	stormkit-core

	INTERFACE LIBRARY_HEADER_ONLY
	INTERFACE STORM_BUILD
)

configure_file(
	${CMAKE_CURRENT_LIST_DIR}/Configure.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/generated/include/storm/core/Configure.hpp
)

storm_export(Core)
