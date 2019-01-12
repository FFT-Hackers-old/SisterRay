find_package(ZLib REQUIRED)

file(GLOB_RECURSE SOURCES "*.c" "*.cpp" "*.h")
add_library(SisterRay SHARED ${SOURCES})
set_target_properties(SisterRay PROPERTIES OUTPUT_NAME "SisterRay")
target_include_directories(SisterRay PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(SisterRay mog ZLib::ZLib)

install(TARGETS SisterRay RUNTIME DESTINATION bin)
