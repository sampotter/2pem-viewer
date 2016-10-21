project (viewer CXX)
cmake_minimum_required (VERSION 3.4)

find_library(fftw3_LIBRARY NAMES fftw3)

find_package (Armadillo REQUIRED)
find_package (Boost QUIET COMPONENTS program_options system REQUIRED)
find_package (FLTK REQUIRED)
find_package (glfw3 3.2 REQUIRED)
find_package (OpenGL REQUIRED)

set (CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++1z")
set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DVIEWER_DEBUG")
set (CMAKE_CXX_FLAGS_RELEASE "-O3")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")

add_subdirectory (src)
