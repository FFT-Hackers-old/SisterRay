cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
option(SUPERBUILD "Build this project using a superbuild" ON)

set(VERSION unknown CACHE STRING "The version of SisterRay")

if (SUPERBUILD)
    project(SUPERBUILD)

    include(ExternalProject)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

    add_subdirectory(third_party)

    ExternalProject_Add(
        SisterRay
        SOURCE_DIR      "${CMAKE_SOURCE_DIR}"
        INSTALL_DIR     "${CMAKE_BINARY_DIR}"
        CMAKE_ARGS
            "-DSUPERBUILD=OFF"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/third_party"
            "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
            "-DVERSION=${VERSION}"
        DEPENDS
        zlib
        lua
    )
    ExternalProject_Add_Step(
		SisterRay
		reconfigure
		COMMAND ${CMAKE_COMMAND} -E echo "Forcing a superbuild reconfigure"
		DEPENDEES download
		DEPENDERS configure
		ALWAYS ON
    )

    install(DIRECTORY "${CMAKE_BINARY_DIR}/bin/" DESTINATION / COMPONENT SisterRay)
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/include" DESTINATION / COMPONENT SisterRay_Dev)

    set(CPACK_OUTPUT_FILE_PREFIX packages)
    set(CPACK_GENERATOR "ZIP;TGZ")
    set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
    set(CPACK_ARCHIVE_SISTERRAY_FILE_NAME "SisterRay-${VERSION}")
    set(CPACK_ARCHIVE_SISTERRAY_DEV_FILE_NAME "SisterRay-Dev-${VERSION}")

    include(CPack)
else()
    project(SisterRay)

    set(CID_NAME        "FF7_SisterRay")
    set(CID_TARGET      "FF7.exe")
    set(CID_DLL         "SisterRay.dll")
    set(CID_INIT        "rayInit")

    add_subdirectory(src)
    add_subdirectory(vendor)
endif()
