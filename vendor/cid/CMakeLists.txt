cmake_minimum_required(VERSION 3.10)
project(Cid)

add_executable(cid cid.c)
target_compile_definitions(
    cid
    PRIVATE -DCID_TARGET=${CID_TARGET}
    PRIVATE -DCID_DLL=${CID_DLL}
    PRIVATE -DCID_INIT=${CID_INIT}
)

set_target_properties(cid PROPERTIES OUTPUT_NAME "${CID_NAME}")
