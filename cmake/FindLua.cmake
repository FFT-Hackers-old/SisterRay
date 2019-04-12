include(FindPackageHandleStandardArgs)

if (NOT LUA_FOUND)
	find_library(
		LUA_LIBRARY
		lua53 lua53d lua luad
		PATH_SUFFIXES
		lib
    )

    find_file(
        LUA_DLL
        lua53.dll lua53d.dll lua.dll luad.dll
        PATH_SUFFIXES
        bin
    )

	find_path(
		LUA_INCLUDE_DIR
		lua/lua.h
		PATH_SUFFIXES
		include
	)

	add_library(Lua::Lua SHARED IMPORTED)

	set_target_properties(
		Lua::Lua
		PROPERTIES
		IMPORTED_LOCATION
        "${LUA_DLL}"
        IMPORTED_IMPLIB
        "${LUA_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES
		"${LUA_INCLUDE_DIR}"
	)

	find_package_handle_standard_args(Lua DEFAULT_MSG LUA_DLL LUA_LIBRARY LUA_INCLUDE_DIR)
endif()
