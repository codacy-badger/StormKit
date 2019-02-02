cmake_minimum_required(VERSION 3.0)

set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)")
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

project(stormkit VERSION 0.2.0 LANGUAGES C CXX)

option(STORM_ENABLE_SHARED                 "Build shared libraries" ON)
option(STORM_ENABLE_PYTHON_WRAPPER         "Build python wrapper" OFF)
option(STORM_ENABLE_STATIC                 "Build static libraries" OFF)
option(STORM_ENABLE_DOC                    "Generate doxygen documentation" OFF)
option(STORM_ENABLE_EXAMPLES               "Build examples" ON)
option(STORM_ENABLE_DEBUG_LOGGING          "Build with logging (all module) only affect when CMAKE_BUILD_TYPE is Debug" OFF)
option(STORM_ENABLE_ENGINE                 "Build storm engine library" ON)
option(STORM_ENABLE_WINDOW                 "Build storm window library" ON)
option(STORM_ENABLE_WINDOW_BACKEND_X11     "Build storm window x11 backend library" ON)
option(STORM_ENABLE_WINDOW_BACKEND_WAYLAND "Build storm window wayland backend library" OFF)
option(STORM_ENABLE_ENTITIES               "Build storm entities library" ON)
option(STORM_ENABLE_IMAGE                  "Build storm image library" ON)
option(STORM_ENABLE_LOG                    "Build storm log library" ON)
option(STORM_ENABLE_TOOLS                  "Build storm tools library" ON)
option(STORM_ENABLE_SHAREDOBJECT           "Build storm sharedobject library" ON)
option(STORM_ENABLE_SWITCHCONTROLLER       "Build storm image library" ON)

include(CMakeDependentOption)

cmake_dependent_option(STORM_ENABLE_ENGINE_VULKAN_BACKEND "Build storm engine vulkan backend" ON
					"NOT STORM_ENABLE_ENGINE_OPENGL_BACKEND" OFF)
cmake_dependent_option(STORM_ENABLE_ENGINE_OPENGL_BACKEND "Build storm engine opengl 3.3 backend" ON
					"NOT STORM_ENABLE_ENGINE_VULKAN_BACKEND" OFF)

include(cmake/GitMacro.cmake)

storm_detect_git_version()

set(STORM_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})
set(STORM_MINOR_VERSION ${PROJECT_VERSION_MINOR})
set(STORM_PATCH_VERSION ${PROJECT_VERSION_PATCH})
set(STORM_MILESTONE     "alpha")
set(STORM_BUILD_TYPE    ${CMAKE_BUILD_TYPE})
set(STORM_GIT_BRANCH    ${GIT_COMMIT_BRANCH})
set(STORM_GIT_VERSION   ${GIT_COMMIT_HASH})

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(STEP_COUNT "3")
if(STORM_ENABLE_DOC)
	set(STEP_COUNT "4")
endif(STORM_ENABLE_DOC)

message(STATUS "---- Configuring StormKit project ----")
message(STATUS "-- Detecting OS 1/${STEP_COUNT}")
include(cmake/OS.cmake)
message(STATUS "-- Checking Compiler 2/${STEP_COUNT}")
include(cmake/C++.cmake)
message(STATUS "-- Checking Dependencies 3/${STEP_COUNT}")
include(cmake/Dependencies.cmake)
message(STATUS "-- Status")
message("-- Options resume:")
message("---------------------------------------------")
message("|STORM_ENABLE_SHARED                 |     ${STORM_ENABLE_SHARED}|")
message("|STORM_ENABLE_PYTHON_WRAPPER         |     ${STORM_ENABLE_PYTHON_WRAPPER}|")
message("|STORM_ENABLE_STATIC                 |     ${STORM_ENABLE_STATIC}|")
message("|STORM_ENABLE_DOC                    |     ${STORM_ENABLE_DOC}|")
message("|STORM_ENABLE_EXAMPLES               |     ${STORM_ENABLE_EXAMPLES}|")
message("|STORM_ENABLE_DEBUG_LOGGING          |     ${STORM_ENABLE_DEBUG_LOGGING}|")
message("|STORM_ENABLE_ENGINE                 |     ${STORM_ENABLE_ENGINE}|")
message("|STORM_ENABLE_WINDOW                 |     ${STORM_ENABLE_WINDOW}|")
message("|STORM_ENABLE_WINDOW_BACKEND_X11     |     ${STORM_ENABLE_WINDOW_BACKEND_X11}|")
message("|STORM_ENABLE_WINDOW_BACKEND_WAYLAND |     ${STORM_ENABLE_WINDOW_BACKEND_WAYLAND}|")
message("|STORM_ENABLE_ENTITIES               |     ${STORM_ENABLE_ENTITIES}|")
message("|STORM_ENABLE_IMAGE                  |     ${STORM_ENABLE_IMAGE}|")
message("|STORM_ENABLE_LOG                    |     ${STORM_ENABLE_LOG}|")
message("|STORM_ENABLE_SHAREDOBJECT           |     ${STORM_ENABLE_SHAREDOBJECT}|")
message("|STORM_ENABLE_TOOLS                  |     ${STORM_ENABLE_TOOLS}|")
message("|STORM_ENABLE_SWITCHCONTROLLER       |     ${STORM_ENABLE_SWITCHCONTROLLER}|")
message("---------------------------------------------")
message("-- Building libs:")
message("--------------------------------------")
message("|Storm::Engine           |         ${STORM_ENABLE_ENGINE}|")
message("|Storm::Window           |         ${STORM_ENABLE_WINDOW}|")
message("|Storm::Entities         |         ${STORM_ENABLE_ENTITIES}|")
message("|Storm::Image            |         ${STORM_ENABLE_IMAGE}|")
message("|Storm::Log              |         ${STORM_ENABLE_LOG}|")
message("|Storm::SharedObject     |         ${STORM_ENABLE_SHAREDOBJECT}|")
message("|Storm::Tools            |         ${STORM_ENABLE_TOOLS}|")
message("|Storm::SwitchController |         ${STORM_ENABLE_SWITCHCONTROLLER}|")
message("--------------------------------------")

include(cmake/ExportMacro.cmake)
include(cmake/C++17Macro.cmake)

add_subdirectory(core/)
add_subdirectory(reflection/)
add_subdirectory(eventbus/)

if(STORM_ENABLE_ENGINE)
	add_subdirectory(extlibs/SPIRV-Cross)
	add_subdirectory(engine/)
endif(STORM_ENABLE_ENGINE)

if(STORM_ENABLE_WINDOW)
	add_subdirectory(window/)
endif(STORM_ENABLE_WINDOW)

if(STORM_ENABLE_ENTITIES)
	add_subdirectory(entities/)
endif(STORM_ENABLE_ENTITIES)

if(STORM_ENABLE_IMAGE)
	add_subdirectory(image/)
endif(STORM_ENABLE_IMAGE)

if(STORM_ENABLE_LOG)
	add_subdirectory(log/)
endif(STORM_ENABLE_LOG)

if(STORM_ENABLE_SHAREDOBJECT)
	add_subdirectory(sharedobject/)
endif(STORM_ENABLE_SHAREDOBJECT)

if(STORM_ENABLE_TOOLS)
	add_subdirectory(tools/)
endif(STORM_ENABLE_TOOLS)


#if(STORM_ENABLE_SWITCHCONTROLLER)
#	add_subdirectory(switchcontroller/)
#endif(STORM_ENABLE_SWITCHCONTROLLER)

include(cmake/CmakeDebug.cmake)

