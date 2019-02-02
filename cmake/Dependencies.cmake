include(CMakeFindDependencyMacro)

#enabling module internal dependencies
if(STORM_ENABLE_IMAGE)
	set(STORM_ENABLE_LOG   ON)
endif()

if(STORM_ENABLE_ENGINE)
	set(STORM_ENABLE_IMAGE ON)
	set(STORM_ENABLE_LOG   ON)
endif()

#search for external dependencies
message("-- - Checking global dependencies")
find_dependency(Threads QUIET REQUIRED)

if(TARGET Threads::Threads)
	message("-- Thread Found")
else()
	message(FATAL "Missing Thread library")
endif()

if(STORM_OS_MACOS)
	message("-- - Checking fallback dependencies")

	find_dependency(Boost QUIET REQUIRED COMPONENTS system filesystem)

	if(TARGET Boost::boost AND TARGET Boost::system AND TARGET Boost::filesystem)
		message("-- Boost Found")
	else()
		message(FATAL_ERROR "Fatal: Missing Boost system or filesystem library")
	endif()

	set(FILESYSTEM_LIBRARIES Boost::boost Boost::system Boost::filesystem)
else()
	set(FILESYSTEM_LIBRARIES -lstdc++fs)
endif()

#search for external dependencies for storm engine module
if(STORM_ENABLE_ENGINE)
	message("-- - Checking Storm::Engine dependencies")

	if(STORM_ENABLE_ENGINE_VULKAN_BACKEND)
		find_dependency(Vulkan QUIET REQUIRED)
		
		set(VK_COMPILE_DEFINITION)
		if(STORM_OS_MACOS)
			set(VK_COMPILE_DEFINITION "VK_USE_PLATFORM_XCB_KHR")
		elseif(STORM_OS_LINUX)
			set(VK_COMPILE_DEFINITION "VK_USE_PLATFORM_MACOS_MVK")
		endif()

		if(DEFINED Vulkan_FOUND)
			message("-- Vulkan Found")
		else()
			message(FATAL_ERROR "Missing Vulkan library")
		endif()
	endif()
	if(STORM_ENABLE_ENGINE_OPENGL_BACKEND)
            set(OpenGL_GL_PREFERENCE "GLVND")
            find_dependency(OpenGL QUIET REQUIRED)

            if(TARGET OpenGL::GL)
                    message("-- OpenGL Found")
            else()
                    message(FATAL_ERROR "Fatal: Missing OpenGL library")
            endif()

            if(STORM_OS_LINUX)
               find_dependency(EGL QUIET REQUIRED)
               if(TARGET EGL::EGL)
                       message("-- EGL Found")
               else()
                       message(FATAL_ERROR "Fatal: Missing EGL library")
               endif()
            endif()
	else()
		message(FATAL "Unknow backend ${STORM_ENGINE_BACKEND}")
	endif()

	find_dependency(glm QUIET REQUIRED)

	if(TARGET glm)
		message("-- GLM Found")
	else()
		message(FATAL_ERROR "Missing GLM library")
	endif()

#	find_dependency(ASSIMP QUIET REQUIRED)
#	message("sojflskfj ${ASSIMP_INCLUDE_DIRS}")
#	if(DEFINED ASSIMP_LIBRARIES)
#		if(STORM_OS_MACOS)
#			set(ASSIMP_FULLNAME "lib${ASSIMP_LIBRARIES}.dylib")
#		elseif(STORM_OS_LINUX)
#			set(ASSIMP_FULLNAME "lib${ASSIMP_LIBRARIES}.so")
#		endif()#todo maje windows compatibility

#		#mapping assimp  ton a target
#		add_library(ASSIMP::ASSIMP SHARED IMPORTED)
#		set_target_properties(ASSIMP::ASSIMP
#			PROPERTIES
#			      INTERFACE_INCLUDE_DIRECTORIES "${ASSIMP_INCLUDE_DIRS}"
#				  IMPORTED_LINK_INTERFACE_LANGUAGES "C"
#				  IMPORTED_LOCATION "${ASSIMP_LIBRARY_DIRS}/${ASSIMP_FULLNAME}"
#			)
#		message("-- ASSIMP Found")
#	else()
#		message(FATAL_ERROR "Fatal: Missing ASSIMP library")
#	endif()
endif()

#search for external dependencies for storm window module
if(STORM_ENABLE_WINDOW)
	message("-- - Checking Storm::Window dependencies")
	if(STORM_OS_WIN32)
		find_dependency(SFML QUIET REQUIRED COMPONENTS system window)

		if(TARGET sfml-system AND TARGET sfml-window)
			message("-- SFML Found")
		else()
			message(FATAL_ERROR "Fatal: Missing SFML library")
		endif()
	elseif(STORM_OS_MACOS)
		find_library(APPKIT_LIBRARY Cocoa)

		if(APPKIT_LIBRARY)
			message("-- Cocoa Found")
		else()
			message(FATAL_ERROR "Fatal: Missing Cocoa library")
		endif()

		find_library(QUARTZ_CORE_LIBRARY QuartzCore)

		if(QUARTZ_CORE_LIBRARY)
			message("-- QuartzCore Found")
		else()
			message(FATAL_ERROR "Fatal: Missing QuartzCore library")
		endif()

		mark_as_advanced(APPKIT_LIBRARY)
	elseif(STORM_OS_LINUX)
		if(STORM_ENABLE_WINDOW_BACKEND_X11)
			find_dependency(XCB QUIET REQUIRED COMPONENTS XCB KEYSYMS ATOM ICCCM)
			if(TARGET XCB::XCB AND TARGET XCB::KEYSYMS AND TARGET XCB::ATOM AND TARGET XCB::ICCCM)
				message("-- XCB Found")
			else()
				message(FATAL_ERROR "Fatal: Missing XCB library")
                        endif()

                        if(STORM_ENABLE_ENGINE_OPENGL_BACKEND)
                            find_dependency(X11 QUIET REQUIRED COMPONENTS X11)
                            if(X11_FOUND)
                                    message("-- X11 Found")
                            else()
                                    message(FATAL_ERROR "Fatal: Missing X11 library")
                            endif()

                            #mapping assimp  ton a target
                            add_library(X11::X11 SHARED IMPORTED)
                            set_target_properties(X11::X11
                                    PROPERTIES
                                          INTERFACE_INCLUDE_DIRECTORIES "${X11_X11_INCLUDE_PATH}"
                                              IMPORTED_LINK_INTERFACE_LANGUAGES "C"
                                              IMPORTED_LOCATION "${X11_X11_LIB}"
                                              )

                          find_dependency(X11_XCB QUIET REQUIRED)
                          if(TARGET X11::XCB)
                                  message("-- X11_XCB Found")
                          else()
                                  message(FATAL_ERROR "Fatal: Missing X11_XCB library")
                          endif()
                        endif()
		elseif(STORM_ENABLE_WINDOW_BACKEND_WAYLAND)
			find_dependency(SFML QUIET REQUIRED COMPONENTS system window)

			if(TARGET sfml-system AND TARGET sfml-window)
				message("-- SFML Found")
			else()
				message(FATAL_ERROR "Fatal: Missing SFML library")
			endif()
		endif()
	endif()
endif()

#search for external dependencies for storm image module
if(STORM_ENABLE_IMAGE)
	message("-- - Checking Storm::Image dependencies")

	find_dependency(PNG QUIET REQUIRED)

	if(TARGET PNG::PNG)
		message("-- PNG Found")
	else()
		message(FATAL_ERROR "Fatal: Missing PNG library")
	endif()

	find_dependency(TURBOJPEG QUIET REQUIRED)

	if(TARGET TURBOJPEG::TURBOJPEG)
		message("-- TURBOJPEG Found")
	else()
		message(FATAL_ERROR "Fatal: Missing TURBOJPEG library")
	endif()
endif()
