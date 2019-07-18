include(ExternalProject)

ExternalProject_Add(
    zlib
    URL                 "${CMAKE_SOURCE_DIR}/third_party/zlib/zlib-1.2.11.tar.gz"
    URL_HASH            "SHA256=c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1"
	INSTALL_DIR			"${CMAKE_BINARY_DIR}/third_party"
	CMAKE_ARGS
		"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
		"-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
        "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
        "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
)
