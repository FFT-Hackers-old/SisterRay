find_package(ZLib REQUIRED)
find_package(Lua REQUIRED)
find_package(Srprotos REQUIRED)

file(GLOB_RECURSE SOURCES "*.c" "*.cpp" "*.h")

add_library(SisterRay SHARED ${SOURCES})
set_target_properties(SisterRay PROPERTIES OUTPUT_NAME "SisterRay")
target_include_directories(SisterRay PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(SisterRay mog ZLib::ZLib Lua::Lua Srprotos::Srprotos)
target_compile_definitions(SisterRay PRIVATE _CRT_SECURE_NO_WARNINGS=1)

install(TARGETS SisterRay RUNTIME DESTINATION bin)
install(FILES "${ZLIB_DLL}" "${LUA_DLL}" DESTINATION bin)
