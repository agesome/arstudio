cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)

project(arstudio CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_SKIP_RULE_DEPENDENCY ON)
set(Boost_USE_MULTITHREADED ON)
set(GLOBAL_LIBS "")
set(ALGO_INCLUDES "")
set(ALGO_CODE "")

set(arstudio_sources
    Config/private/Config.cpp
    Logger/private/Logger.cpp
    Repository/private/Repository.cpp
    Scenegraph/private/Scenegraph.cpp
    Sequence/private/Sequence.cpp
    VideoHelper/private/VideoHelper.cpp
    VideoHelper/private/VideoSourceOpenCV.cpp
    VideoHelper/private/VideoSourceKinvideo.cpp
    ${CMAKE_BINARY_DIR}/AlgoPipeline.cpp
    pipeline.cpp
    ${CMAKE_BINARY_DIR}/main.cpp

    Core/Core.qml
    Core/MainMenuBar.qml
    Core/MainToolBar.qml
    TimeLine/TimeLine.qml
    WindowTool/Axis.qml
    WindowTool/ItemWindow.qml
    WindowTool/WindowHandler.qml
    WindowTool/WindowTool.qml
)

# this is somewhat tricky. we want a separate list of all headers so that they
# show up for those who use Qt Creator, and we want a list of Qt-using class
# headers to process them with moc; we cannot just process all the headers,
# that will produce warnings

set(arstudio_headers
    Config/Config.hpp
    Logger/Logger.hpp
    Sequence/Sequence.hpp
    Sequence/PointCloud.hpp
    Sequence/Bitmap.hpp
    Sequence/Camera.hpp
    Sequence/Point3D.hpp
    Repository/Repository.hpp
    Scenegraph/Scenegraph.hpp
    VideoHelper/VideoHelper.hpp
    VideoHelper/IVideoSource.hpp
    VideoHelper/VideoSourceOpenCV.hpp
    VideoHelper/VideoSourceKinvideo.hpp
)

set(arstudio_moc_headers
    pipeline.hpp
    Scenegraph/Scenegraph.hpp
    Sequence/Bitmap.hpp
    Sequence/Camera.hpp
    Sequence/Point3D.hpp
    Sequence/PointCloud.hpp
    Sequence/Sequence.hpp
    Repository/Repository.hpp
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
  "Choose the type of build: Debug/Release" FORCE)
endif()

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt53DQuick REQUIRED)
find_package(OpenCV REQUIRED)

add_definitions(${Qt5Quick_DEFINITIONS})
add_definitions(${Qt53DQuick_DEFINITIONS})
add_definitions(${Qt5Concurrent_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Quick_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt53DQuick_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Concurrent_EXECUTABLE_COMPILE_FLAGS}")

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -ggdb3 -Wall -Wextra -pedantic")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
endif()

add_subdirectory(../submodules/KinectRecorder/src/
                 ${CMAKE_CURRENT_BINARY_DIR}/KinectRecorder
                 EXCLUDE_FROM_ALL)

include_directories(
  AlgoPipeline
  Logger
  Config
  Repository
  Scenegraph
  Sequence
  VideoHelper
  ../submodules/KinectRecorder/src/
  ${Boost_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${Qt5Quick_INCLUDE_DIRS}
  ${Qt53DQuick_INCLUDE_DIRS}
  ${Qt5Concurrent_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLOBAL_INCLUDES}
)

macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
  if(IS_DIRECTORY ${curdir}/${child})
    set(dirlist ${dirlist} ${child})
  endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

if(NOT ALGORITHMS)
  subdirlist(subdirs algorithms/)
    set(ALGORITHMS ${subdirs} CACHE STRING
    "List algorithms to build" FORCE)
endif()

message (STATUS "Algorithms to build: " ${ALGORITHMS})

if (NOT ALGORITHMS STREQUAL "")
  message (STATUS "Will now add algorithms")
  foreach(subdir ${ALGORITHMS})
    message (STATUS "  Adding algorithm "${subdir})
    include_directories(algorithms/${subdir}/)
    if (IS_DIRECTORY algorithms/${subdir}/include/)
      include_directories(algorithms/${subdir}/include/)
    endif ()
    if (IS_DIRECTORY algorithms/${subdir}/src/)
      include_directories(algorithms/${subdir}/src/)
      add_subdirectory(algorithms/${subdir}/src/
          ${CMAKE_CURRENT_BINARY_DIR}/${subdir} EXCLUDE_FROM_ALL)
    else ()
      add_subdirectory(algorithms/${subdir}/
          ${CMAKE_CURRENT_BINARY_DIR}/${subdir} EXCLUDE_FROM_ALL)
    endif ()
  endforeach()
endif ()

set(CONFIG_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/")
set(QML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/")

configure_file(main.cpp ${CMAKE_BINARY_DIR}/main.cpp)

configure_file(AlgoPipeline/AlgoPipeline.hpp
               ${CMAKE_BINARY_DIR}/AlgoPipeline.hpp)

configure_file(AlgoPipeline/private/AlgoPipeline.cpp
               ${CMAKE_BINARY_DIR}/AlgoPipeline.cpp)

qt5_wrap_cpp(arstudio_moc ${arstudio_moc_headers})
qt5_add_resources(arstudio_res resources/list.qrc)

message(STATUS "Build type is: ${CMAKE_BUILD_TYPE}")

add_executable(arstudio
               ${arstudio_sources}
               ${arstudio_headers}
               ${arstudio_moc}
               ${arstudio_res}
)

qt5_use_modules(arstudio Quick 3DQuick Concurrent)
target_link_libraries(
  arstudio

  ${GLOBAL_LIBS}
  ${Boost_LIBRARIES}
  ${OpenCV_LIBS}
)
