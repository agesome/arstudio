cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(cvdbg CXX C)
set(CMAKE_AUTOMOC TRUE)

set(cvdbg_sources

	cvdbg.cpp
	sequence.cpp
	repository.cpp
	scenegraph.cpp
<<<<<<< HEAD
        window3d.cpp
        camera.cpp
        point3d.cpp
	scenegraph_selector.cpp
        logger.cpp
        timeline.cpp
        timelinemodel.cpp
        core.cpp)
=======
	window3d.cpp
	camera.cpp
	point3d.cpp
	repository_view.cpp
	logger.cpp
	timeline.cpp
	core.cpp)
>>>>>>> 86866e2bc19b9b471850248534400c8101f4ded7

set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL)
include(${QT_USE_FILE})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

<<<<<<< HEAD
include_directories("./")
add_definitions("-std=c++11 -ggdb3")
=======
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "-std=c++11")
>>>>>>> 86866e2bc19b9b471850248534400c8101f4ded7

add_executable(cvdbg ${cvdbg_sources})
target_link_libraries(cvdbg ${QT_LIBRARIES} ${Boost_LIBRARIES} ${OPENGL_LIBRARIES})
