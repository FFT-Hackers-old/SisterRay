include(ExternalProject)

ExternalProject_Add(
    lua
    URL                 "${CMAKE_SOURCE_DIR}/third_party/lua/lua-5.3.5.tar.gz"
    URL_HASH            "SHA256=0C2EED3F960446E1A3E4B9A1CA2F3FF893B6CE41942CF54D5DD59AB4B3B058AC"
    INSTALL_DIR			"${CMAKE_BINARY_DIR}/third_party"
    PATCH_COMMAND       "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/third_party/lua/CMakeLists.txt.in" <SOURCE_DIR>/CMakeLists.txt
    CMAKE_ARGS
		"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
		"-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
)
