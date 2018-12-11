add_library(SisterRay SHARED init.c)
set_target_properties(SisterRay PROPERTIES OUTPUT_NAME "SisterRay")
target_link_libraries(SisterRay mog)
