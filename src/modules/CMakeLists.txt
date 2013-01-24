find_package(OpenCV REQUIRED)

include_directories ("/usr/include/ni/")
find_library (lib_openni OpenNI)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ClassKin)

#set(contrast_src contrast.cpp)
#add_library(contrast MODULE ${contrast_src})
#set_target_properties(contrast PROPERTIES PREFIX "")

set(brightness_src brightness.cpp)
add_library(brightness MODULE ${brightness_src})
set_target_properties(brightness PROPERTIES PREFIX "")