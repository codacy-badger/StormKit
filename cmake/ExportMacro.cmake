macro(storm_append_prefix_and_suffix NAME)
	set(STORM_SUFFIX "")
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		set(STORM_SUFFIX "d")
	endif()

	if(STORM_ENABLE_STATIC)
		set_target_properties(stormkit-${NAME} PROPERTIES PREFIX "lib" SUFFIX "${STORM_SUFFIX}s${CMAKE_STATIC_LIBRARY_SUFFIX}")
	else()
		set_target_properties(stormkit-${NAME} PROPERTIES PREFIX "lib" SUFFIX "${STORM_SUFFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}")
	endif()
endmacro()

macro(storm_export NAME)
	set (extra_macro_args ${ARGN})
	list(LENGTH extra_macro_args extra_args_count)

	set(FIND_DEPENDENCY_FILES ${${ARGV1}})

	string(TOLOWER ${NAME} NAME_LOWER)
	set_target_properties(stormkit-${NAME_LOWER} PROPERTIES EXPORT_NAME ${NAME_LOWER})

	install(
		TARGETS stormkit-${NAME_LOWER}
		EXPORT  stormkit-${NAME_LOWER}-targets

		RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
		PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

	install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

	install(
		EXPORT stormkit-${NAME_LOWER}-targets
		FILE StormKit${NAME}Targets.cmake
		NAMESPACE StormKit::
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StormKit
	)

	write_basic_package_version_file(
		${CMAKE_CURRENT_BINARY_DIR}/StormKit${NAME}ConfigVersion.cmake
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY AnyNewerVersion
	)

	configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/StormKit${NAME}Config.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/StormKit${NAME}Config.cmake
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StormKit
	)

	install(
		FILES
		${CMAKE_CURRENT_BINARY_DIR}/StormKit${NAME}Config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/StormKit${NAME}ConfigVersion.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StormKit
	)

	foreach(FILE IN LISTS FIND_DEPENDENCY_FILES)
		install(FILES
			"${stormkit_SOURCE_DIR}/cmake/Modules/${FILE}"
			 DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StormKit
		)

		configure_file(
			"${stormkit_SOURCE_DIR}/cmake/Modules/${FILE}"
			"${CMAKE_CURRENT_BINARY_DIR}/${FILE}"
			COPYONLY
		)
	endforeach()

	export(EXPORT stormkit-${NAME_LOWER}-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/StormKit${NAME}Targets.cmake NAMESPACE StormKit::)
	export(PACKAGE StormKit)
endmacro()
