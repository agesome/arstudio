cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(arstudio CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_SKIP_RULE_DEPENDENCY ON)

set(arstudio_sources
    ${CMAKE_BINARY_DIR}/main.cpp
    Config/private/Config.cpp
    Logger/private/Logger.cpp
    Repository/private/Repository.cpp
    Scenegraph/private/Scenegraph.cpp
    Scenegraph/private/ScenegraphAggregator.cpp
    Sequence/private/Sequence.cpp
    VideoHelper/private/VideoHelper.cpp
    VideoHelper/private/VideoSourceOpenCV.cpp
    VideoHelper/private/VideoSourceKinvideo.cpp
    AlgoPipeline/private/AlgoPipeline.cpp
    AlgoPipeline/private/AlgorithmFactory.cpp
    IWManager/private/IWManager.cpp
    VideoPipeline/private/VideoPipeline.cpp
    CustomModel/private/CustomModel.cpp
    BitmapView/private/BitmapView.cpp
    RepositoryIO/private/RepositoryIO.cpp
    Sequence/private/ItemImplementations.cpp

    Core/Core.qml
    Core/MainMenuBar.qml
    TimeLine/TimeLine.qml
    WindowTool/Axis.qml
    WindowTool/ItemWindow.qml
    WindowTool/WindowHandler.qml
    WindowTool/WindowTool.qml
    WindowTool/ModelControls.qml
    ProcessingTool/ProcessingTool.qml
    ConfigView/ConfigView.qml
)

# this is somewhat tricky. we want a separate list of all headers so that they
# show up for those who use Qt Creator, and we want a list of Qt-using class
# headers to process them with moc; we cannot just process all the headers,
# that will produce warnings

set(arstudio_headers
    AlgoPipeline/AlgoInterface.hpp
    AlgoPipeline/AlgoPipeline.hpp
    Config/Config.hpp
    Logger/Logger.hpp
    Sequence/Sequence.hpp
    Sequence/PointCloud.hpp
    Sequence/Bitmap.hpp
    Sequence/Camera.hpp
    Sequence/Point3D.hpp
    Repository/Repository.hpp
    Repository/RepositoryNode.hpp
    Scenegraph/Scenegraph.hpp
    Scenegraph/ScenegraphAggregator.hpp
    VideoHelper/VideoHelper.hpp
    VideoHelper/IVideoSource.hpp
    VideoHelper/VideoSourceOpenCV.hpp
    VideoHelper/VideoSourceKinvideo.hpp
    IWManager/IWManager.hpp
    VideoPipeline/VideoPipeline.hpp
    CustomModel/CustomModel.hpp
    BitmapView/BitmapView.hpp
    AlgoPipeline/AlgorithmFactory.hpp
    RepositoryIO/RepositoryIO.hpp
)

set(arstudio_moc_headers
    Scenegraph/Scenegraph.hpp
    Scenegraph/ScenegraphAggregator.hpp
    Sequence/Sequence.hpp
    Repository/Repository.hpp
    IWManager/IWManager.hpp
    VideoPipeline/VideoPipeline.hpp
    VideoHelper/VideoHelper.hpp
    Config/Config.hpp
    CustomModel/CustomModel.hpp
    BitmapView/BitmapView.hpp
    Repository/RepositoryNode.hpp
)

set(resource_files
    resources/list.qrc
    qml-files.qrc
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
  "Choose the type of build: Debug/Release" FORCE)
endif()

find_package(Qt5 REQUIRED Quick 3DQuick Concurrent)
find_package(OpenCV REQUIRED)

find_library(MAGIC magic)

message(STATUS "Qt version: ${Qt5Core_VERSION_STRING}")
message(STATUS "OpenCV version: ${OpenCV_VERSION}")

if(MAGIC STREQUAL "LIBMAGIC-NOTFOUND")
    message(STATUS "Use libmagic: NO")
else()
    message(STATUS "Use libmagic: YES")
    add_definitions(-DHAVE_LIBMAGIC)
    set(LIBMAGIC ${MAGIC})
endif()

if(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 e-Wall -Wextra -pedantic")
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS} -std=c++11 -Ofast -march=native -mtune=native -DQT_NO_DEBUG")
else()
  message(FATAL_ERROR "There is no support for building on non-unix systems (yet).")
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
  IWManager
  VideoPipeline
  CustomModel
  BitmapView
  RepositoryIO
  ../submodules/KinectRecorder/src/
  ${OpenCV_INCLUDE_DIRS}
  ${Qt5Quick_INCLUDE_DIRS}
  ${Qt53DQuick_INCLUDE_DIRS}
  ${Qt5Concurrent_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  foreach(child ${children})
  if(IS_DIRECTORY ${curdir}/${child})
    list(APPEND dirlist ${child})
  endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

if(NOT ALGORITHMS)
  subdirlist(subdirs ${CMAKE_SOURCE_DIR}/algorithms/)
    set(ALGORITHMS ${subdirs} CACHE STRING
    "List algorithms to build" FORCE)
endif()

message(STATUS "Algorithms to build: ${subdirs}")

if(NOT ALGORITHMS STREQUAL "")
  message(STATUS "Will now add algorithms")
  foreach(subdir ${ALGORITHMS})
    message(STATUS "  Adding algorithm "${subdir})
      add_subdirectory(${CMAKE_SOURCE_DIR}/algorithms/${subdir}/
          ${CMAKE_CURRENT_BINARY_DIR}/${subdir}
          EXCLUDE_FROM_ALL)
  endforeach()
endif()

set(CONFIG_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/")

configure_file(main.cpp ${CMAKE_BINARY_DIR}/main.cpp)

qt5_wrap_cpp(arstudio_moc ${arstudio_moc_headers})
qt5_add_resources(arstudio_res ${resource_files})

message(STATUS "Build type is: ${CMAKE_BUILD_TYPE}")

add_executable(arstudio
               ${arstudio_sources}
               ${arstudio_headers}
               ${arstudio_moc}
               ${arstudio_res}
)

target_link_libraries(
  arstudio
  kinectCapture
  ${ALGORITHMS}
  ${OpenCV_LIBS}
  ${LIBMAGIC}
  Qt5::Quick
  Qt5::3DQuick
  Qt5::Concurrent
)
