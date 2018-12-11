cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE SOURCES "src/*.c" "include/*.h")
add_library(mog STATIC ${SOURCES})
target_include_directories(mog PUBLIC include)
