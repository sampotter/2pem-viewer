set (client_srcs
  viewer/client.cpp
  viewer/client_app_state.hpp
  viewer/client_app_state.cpp
  viewer/client_asio_state.hpp
  viewer/client_asio_state.cpp
  viewer/client_gl_state.hpp
  viewer/client_gl_state.cpp
  viewer/client_glfw_state.hpp
  viewer/client_glfw_state.cpp
  viewer/client_input_window.hpp
  viewer/client_input_window.cpp
  viewer/client_options.hpp
  viewer/client_options.cpp
  viewer/client_signal_dispatcher.hpp
  viewer/client_signal_dispatcher.cpp
  viewer/client_slm_window.hpp
  viewer/client_slm_window.cpp
  viewer/frame.hpp
  viewer/frame.cpp
  viewer/glfw.hpp
  viewer/glfw.cpp
  viewer/gl.hpp
  viewer/gl.cpp)
set (server_srcs
  viewer/server.cpp
  viewer/raw_file.hpp
  viewer/raw_file.impl.hpp)
set (viewer_srcs viewer/main.cpp)

add_executable (client ${client_srcs})
target_link_libraries (client ${fftw3_LIBRARY})
target_link_libraries (client ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries (client ${Boost_SYSTEM_LIBRARY})
target_link_libraries (client glfw)
target_link_libraries (client ${OPENGL_gl_LIBRARY})

add_executable (server ${server_srcs})
target_link_libraries (server ${Boost_SYSTEM_LIBRARY})

add_executable (viewer ${viewer_srcs})
target_link_libraries (viewer glfw)
target_link_libraries (viewer ${OPENGL_gl_LIBRARY})

add_executable (hello viewer/hello.cpp)
target_link_libraries (hello ${FLTK_LIBRARY})

# Local Variables:
# indent-tabs-mode: nil
# End:
