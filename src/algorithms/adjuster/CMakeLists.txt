cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(adjuster CXX)

set(sources adjuster.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(adjuster SHARED ${sources})

if (DEFINED GLOBAL_LIBS)
	set(GLOBAL_LIBS ${GLOBAL_LIBS} adjuster; PARENT_SCOPE)
endif()

if (DEFINED ALGO_INCLUDES)
	set(ALGO_INCLUDES "${ALGO_INCLUDES}#include <adjuster.hpp>\n" PARENT_SCOPE)
endif ()

if (DEFINED ALGO_CODE)
	set(ALGO_CODE "${ALGO_CODE}algo_list.push_back (new ImageAdjusterAlgorithm(config));\n"
		PARENT_SCOPE)
endif()
