project(StormKitWindow VERSION 0.2.0 LANGUAGES C CXX)

add_library(
	stormkit-window
	SHARED
	    src/Window.cpp
		src/AbstractWindow.cpp

		include/storm/window/Window.hpp
		include/storm/window/AbstractWindow.hpp
		include/storm/window/WindowStyle.hpp
		include/storm/window/Key.hpp
		include/storm/window/Event.hpp
		include/storm/window/MouseButton.hpp
		include/storm/window/VideoSettings.hpp
)

add_library(StormKit::window ALIAS stormkit-window)

if(STORM_OS_WIN32)
	target_sources(
		stormkit-window
		PRIVATE
		    src/win32/WindowImpl.hpp
			src/win32/WindowImpl.cpp
	)

    target_link_libraries(
		stormkit-window
		PRIVATE
		    sfml-system
			sfml-window
	)
elseif(STORM_OS_MACOS)
	set(OBJCPP_FILES
		src/macOS/AutoReleasePoolWrapper.h
		src/macOS/AutoReleasePoolWrapper.mm
		src/macOS/StormView.h
		src/macOS/StormView.mm
		src/macOS/StormWindowController.h
		src/macOS/StormWindowController.mm
		src/macOS/Utils.h
		src/macOS/Utils.mm
		src/macOS/WindowImpl.h
		src/macOS/WindowImpl.mm
	)
    set_source_files_properties(
		${OBJCPP_FILES}
		PROPERTIES
		    COMPILE_FLAGS "-x objective-c++"
			LANGUAGE      CXX
	)
    set(OBJC_FILES
		src/macOS/StormApplication.h
		src/macOS/StormApplication.m
		src/macOS/StormApplicationDelegate.h
		src/macOS/StormApplicationDelegate.m
		src/macOS/StormWindow.h
		src/macOS/StormWindow.m
	)
    set_source_files_properties(
		${OBJC_FILES}
		PROPERTIES
		    COMPILE_FLAGS "-x objective-c"
			LANGUAGE      C
	)
    target_sources(
		stormkit-window
		PRIVATE
		    ${OBJCPP_FILES}
			${OBJC_FILES}
	)

	target_link_libraries(
		stormkit-window
		PRIVATE
			${APPKIT_LIBRARY}
			${QUARTZ_CORE_LIBRARY}
	)
    target_compile_options(
		stormkit-window
		PRIVATE
		-Wno-pragma-once-outsite-header
	)
elseif(STORM_OS_LINUX)
	if(STORM_ENABLE_WINDOW_BACKEND_X11)
		target_sources(
			stormkit-window
			PRIVATE
			    src/x11/WindowImpl.hpp
				src/x11/WindowImpl.cpp
				src/x11/Utils.hpp
				src/x11/Utils.cpp
		)

	    target_link_libraries(
			stormkit-window
			PRIVATE
			    XCB::XCB
                            XCB::KEYSYMS
                            XCB::ICCCM
		)
	    target_compile_definitions(
			stormkit-window
			PRIVATE
			    -DSTORM_X11
		)
    elseif(STORM_ENABLE_WINDOW_BACKEND_WAYLAND)
		target_sources(
			stormkit-window
			PRIVATE
			    src/wayland/WindowImpl.hpp
				src/wayland/WindowImpl.cpp
		)

	    target_link_libraries(
			stormkit-window
			PRIVATE
			    sfml-system
				sfml-window
		)
	    target_compile_definitions(
			stormkit-window
			PRIVATE
			    -DSTORM_WAYLAND
		)
    endif()
endif()

target_link_libraries(
	stormkit-window
	PUBLIC
	    StormKit::core
)

target_include_directories(
	stormkit-window
	PUBLIC
	    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

enable_cpp17(stormkit-window)
storm_append_prefix_and_suffix(window)
storm_export(Window)

if(STORM_ENABLE_EXAMPLES)
	add_subdirectory(examples/hello_window)
endif()
