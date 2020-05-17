include(ExternalProject)

ExternalProject_Add(
  proto
  URL                    https://github.com/google/protobuf/archive/v3.12.0.tar.gz
  URL_HASH               "SHA256=946ba5371e423e1220d2cbefc1f65e69a1e81ca5bab62a03d66894172983cfcd"
  INSTALL_DIR			 "${CMAKE_BINARY_DIR}/third_party"
  SOURCE_SUBDIR          cmake
  CMAKE_ARGS
		"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
		"-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
        "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
        "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
        "-Dprotobuf_BUILD_TESTS=OFF"
)
