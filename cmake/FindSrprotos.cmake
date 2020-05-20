include(FindPackageHandleStandardArgs)

if (NOT SRPROTOS_FOUND)
	find_library(
		SRPROTOS_LIBRARY
		srprotos
		PATH_SUFFIXES
		lib
    )

	add_library(Srprotos::Srprotos STATIC IMPORTED)

	set_target_properties(
		Srprotos::Srprotos
		PROPERTIES
        IMPORTED_LOCATION
        "${SRPROTOS_LIBRARY}"
	)

	find_package_handle_standard_args(Srprotos DEFAULT_MSG SRPROTOS_LIBRARY)
endif()
