cmake_minimum_required(VERSION 2.8)

project(dispatch_example)

include_directories(./)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  set(DEBUG_FLAGS "/GS /W3 /Gm- /Zi /Od /Oy- /Ob0 /D\"_DEBUG\" /MDd")
  set(RELEASE_FLAGS "/GS /GL /W3 /Gy /Zi /O2 /MD")
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
else()
  set(DEBUG_FLAGS "-g -O0 -Wall -Werror -pedantic -std=c++1z")
  set(RELEASE_FLAGS "-O3 -Wall -Werror -pedantic -std=c++1z")
endif()

set(CMAKE_CXX_FLAGS_DEBUG ${DEBUG_FLAGS})
set(CMAKE_CXX_FLAGS_RELEASE ${RELEASE_FLAGS})

set(CMAKE_CONFIGURATION_TYPES Debug Release)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message(STATUS "Build type unspecified. Defaulting type: ${CMAKE_BUILD_TYPE}")
endif (NOT CMAKE_BUILD_TYPE)


file(GLOB_RECURSE BAD_SRC RELATIVE_PATH
      bad_dispatch/*.h)
source_group("bad_dispatch" FILES ${BAD_SRC})

file(GLOB_RECURSE BETTER_SRC RELATIVE_PATH
      better_dispatch/*.h)
source_group("better_dispatch" FILES ${BETTER_SRC})

file(GLOB_RECURSE COMMON_SRC RELATIVE_PATH
      common/*.h)
source_group("common" FILES ${COMMON_SRC})

file(GLOB_RECURSE EVEN_BETTER_SRC RELATIVE_PATH
      even_better_dispatch/*.h)
source_group("even_better_dispatch" FILES ${EVEN_BETTER_SRC})

file(GLOB_RECURSE VISITOR_SRC RELATIVE_PATH
      visitor_dispatch/*.h)
source_group("visitor_dispatch" FILES ${VISITOR_SRC})

set(EXAMPLE_SRC main.cpp
                  ${BAD_SRC}
                  ${BETTER_SRC}
                  ${COMMON_SRC}
                  ${EVEN_BETTER_SRC}
                  ${VISITOR_SRC})

add_executable(dispatch_example ${EXAMPLE_SRC})

set_property(TARGET dispatch_example PROPERTY FOLDER "dispatch_example")
