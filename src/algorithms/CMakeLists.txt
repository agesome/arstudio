find_package(OpenCV REQUIRED)

include_directories ("/usr/include/ni/")
find_library (lib_openni OpenNI)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ClassKin)

set(adjuster_src adjuster.cpp)
add_library(adjuster STATIC ${adjuster_src})
#set_target_properties(adjuster PROPERTIES PREFIX "")