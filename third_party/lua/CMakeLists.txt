include(ExternalProject)

ExternalProject_Add(
    lua
    URL                 "https://github.com/lua/lua/archive/v5.3.5.tar.gz"
    URL_HASH            "SHA256=3c1bce24bf184c36fde52d8b8e5b9a633b3b0921a5bf19faefefbce02579e9c6"
    INSTALL_DIR			"${CMAKE_BINARY_DIR}/third_party"
    PATCH_COMMAND       "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/third_party/lua/CMakeLists.txt.in" <SOURCE_DIR>/CMakeLists.txt
    CMAKE_ARGS
		"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
		"-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
        "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
        "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
)
