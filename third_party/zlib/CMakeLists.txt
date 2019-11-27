include(ExternalProject)

ExternalProject_Add(
    zlib
    URL                 "https://github.com/madler/zlib/archive/v1.2.11.tar.gz"
	INSTALL_DIR			"${CMAKE_BINARY_DIR}/third_party"
	CMAKE_ARGS
		"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
		"-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
        "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
        "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
)
