project(StormKitSharedObject VERSION 0.2.0 LANGUAGES C CXX)

add_library(
	stormkit-sharedobject
    SHARED
        src/SharedLibrary.cpp
        include/storm/sharedobject/SharedLibrary.hpp
        $<$<BOOL:${STORM_OS_WIN32}>:src/impl/Win32/SharedLibraryImpl.cpp>
        $<$<BOOL:${STORM_OS_WIN32}>:src/impl/Win32/SharedLibraryImpl.hpp>
        $<$<BOOL:${STORM_OS_POSIX}>:src/impl/Posix/SharedLibraryImpl.cpp>
        $<$<BOOL:${STORM_OS_POSIX}>:src/impl/Posix/SharedLibraryImpl.hpp>
)

add_library(StormKit::sharedobject ALIAS stormkit-sharedobject)

if(STORM_OS_WIN32)
    target_sources(stormkit-sharedobject PRIVATE src/impl/Win32/SharedLibraryImpl.cpp src/impl/Win32/SharedLibraryImpl.hpp)
elseif(STORM_OS_POSIX)
    target_sources(stormkit-sharedobject PRIVATE src/impl/Posix/SharedLibraryImpl.cpp src/impl/Posix/SharedLibraryImpl.hpp)
endif()

target_link_libraries(
	stormkit-sharedobject
	PUBLIC
		StormKit::core
)



target_include_directories(
	stormkit-sharedobject
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

enable_cpp17(stormkit-sharedobject)
storm_append_prefix_and_suffix(sharedobject)
storm_export(SharedObject)

