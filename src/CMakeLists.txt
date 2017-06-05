set (client_srcs
  viewer/client.cpp
  viewer/client_app_state.cpp
  viewer/client_asio_state.cpp
  viewer/client_input_window.cpp
  viewer/client_input_window_gl_state.cpp
  viewer/client_options.cpp
  viewer/client_signal_dispatcher.cpp
  viewer/client_slm_state.cpp
  viewer/client_slm_window.cpp
  viewer/client_slm_window_gl_state.cpp
  viewer/frame.cpp
  viewer/phase_retrieval.cpp
  viewer/glfw.cpp
  viewer/gl.cpp
  viewer/lua.cpp
  viewer/target_point.cpp
  viewer/target_point_store.cpp)

if (USE_AUDIO_INPUT)
  set (client_srcs ${client_srcs} viewer/client_audio_input.cpp)
endif ()

if (USE_OSC)
  set (client_srcs ${client_srcs} viewer/client_osc_state.cpp)
endif ()

add_executable (client ${client_srcs})
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

add_executable (server viewer/server.cpp)
target_link_libraries (server ${Boost_SYSTEM_LIBRARY})
target_link_libraries (server ${Boost_IOSTREAMS_LIBRARY})
if (${WINDOWS})
  target_link_libraries (server wsock32)
  target_link_libraries (server ws2_32)
endif ()

# Local Variables:
# indent-tabs-mode: nil
# End:
