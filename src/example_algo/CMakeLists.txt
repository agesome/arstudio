include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

set(adjuster_src adjuster.cpp)
add_library(adjuster STATIC ${adjuster_src})
