cmake_minimum_required(VERSION 3.15)
project(SRPROTOS)

find_package(protobuf REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/protos")
file(GLOB SOURCE_FILES *.cc)
file(GLOB HEADER_FILES *.h)

add_library(srprotos STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(srprotos PUBLIC ${PROTOBUF_INCLUDE_DIR})
target_link_libraries(srprotos protobuf::libprotobuf)
install(TARGETS srprotos)
