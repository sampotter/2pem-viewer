project (viewer CXX)
cmake_minimum_required (VERSION 3.4)

option (USE_AUDIO_INPUT "Build audio input support for triggering" OFF)
option (USE_OSC "Build OSC support for triggering" ON)
option (USE_PORT_AUDIO "Use PortAudio for audio input" ON)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set (LINUX 1)
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set (WINDOWS 1)
endif ()

if (APPLE)
  set (OSX 1)
endif ()

if ((${LINUX}) OR (${WINDOWS}))
  set (LINUX_OR_WINDOWS 1)
endif ()

find_library (fftw3_LIBRARY NAMES fftw3)
find_library (lo_LIBRARY NAMES lo)
find_library (portaudio_LIBRARY NAMES portaudio)
if (${OSX})
  find_library (CoreFoundation_LIBRARY CoreFoundation)
  find_library (CoreServices_LIBRARY CoreServices)
endif ()

find_package (Armadillo REQUIRED)
find_package (Boost QUIET COMPONENTS filesystem program_options system thread
  REQUIRED)
find_package (glfw3 3.2 REQUIRED)
find_package (OpenGL REQUIRED)
if (${LINUX_OR_WINDOWS})
  find_package (GLEW REQUIRED)
endif ()

include_directories (${OPENGL_INCLUDE_DIR})

set (CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++1z")
set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DVIEWER_DEBUG")
set (CMAKE_CXX_FLAGS_RELEASE "-O3")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")

include_directories (${PROJECT_BINARY_DIR}/src/viewer)

add_subdirectory (src)
add_subdirectory (src/viewer)

# Local Variables:
# indent-tabs-mode: nil
# End:
