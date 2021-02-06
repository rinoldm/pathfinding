PROJECT(pathfinding)

cmake_minimum_required(VERSION 3.19.4)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror=switch -pedantic -std=c++11")
  IF (CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  ENDIF (CMAKE_BUILD_TYPE MATCHES DEBUG)
endif ()

set(CMAKE_SOURCE_DIR .)

set(
  MAIN_SOURCES
  Dijkstra.cpp
  Dijkstra.hh
  Heldkarp.cpp
  Heldkarp.hh
  Laby.cpp
  Laby.hh
  main.cpp
  Parser.cpp
  Parser.hh
)
add_executable(pathfinding ${MAIN_SOURCES})
