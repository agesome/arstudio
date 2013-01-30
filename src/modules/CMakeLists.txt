find_package(OpenCV REQUIRED)

include_directories ("/usr/include/ni/")
find_library (lib_openni OpenNI)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ClassKin)

set(brightness_src brightness.cpp)
add_library(brightness MODULE ${brightness_src})
set_target_properties(brightness PROPERTIES PREFIX "")