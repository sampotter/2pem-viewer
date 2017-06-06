set (dsp_srcs
  dsp/frame.cpp
  dsp/phase_retrieval.cpp)

add_library (dsp ${dsp_srcs})

set (client_srcs
  glclient/main.cpp
  glclient/app_state.cpp
  glclient/asio_state.cpp
  glclient/input_window.cpp
  glclient/input_window_gl_state.cpp
  glclient/options.cpp
  glclient/signal_dispatcher.cpp
  glclient/slm_state.cpp
  glclient/slm_window.cpp
  glclient/slm_window_gl_state.cpp
  glclient/glfw.cpp
  glclient/gl.cpp
  glclient/lua.cpp
  glclient/target_point.cpp
  glclient/target_point_store.cpp)

if (USE_AUDIO_INPUT)
  set (client_srcs ${client_srcs} glclient/audio_input.cpp)
endif ()

if (USE_OSC)
  set (client_srcs ${client_srcs} glclient/osc_state.cpp)
endif ()

add_executable (client ${client_srcs})
target_link_libraries (client dsp)
target_link_libraries (client ${ARMADILLO_LIBRARIES})
target_link_libraries (client ${fftw3_LIBRARY})
if (USE_PORT_AUDIO)
  target_link_libraries (client ${portaudio_LIBRARY})
endif ()
if (USE_OSC)
  target_link_libraries (client ${lo_LIBRARY})
endif ()
target_link_libraries (client ${Boost_FILESYSTEM_LIBRARY})
target_link_libraries (client ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries (client ${Boost_SYSTEM_LIBRARY})
if (${WINDOWS})
  target_link_libraries (client wsock32)
  target_link_libraries (client ws2_32)
endif ()
if (${LINUX_OR_WINDOWS})
  target_link_libraries (client ${GLEW_LIBRARIES})
endif ()
target_link_libraries (client glfw)
target_link_libraries (client ${LUA_LIBRARY})
target_link_libraries (client ${OPENGL_gl_LIBRARY})

add_executable (server server/main.cpp)
target_link_libraries (server ${Boost_SYSTEM_LIBRARY})
target_link_libraries (server ${Boost_IOSTREAMS_LIBRARY})
if (${WINDOWS})
  target_link_libraries (server wsock32)
  target_link_libraries (server ws2_32)
endif ()

add_executable (wx wx/main.cpp)
target_link_libraries (wx ${wxWidgets_LIBRARIES})

# Local Variables:
# indent-tabs-mode: nil
# End:
