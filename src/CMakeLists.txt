set (client_srcs
  viewer/client.cpp
  viewer/glfw.hpp
  viewer/glfw.cpp
  viewer/gl.hpp
  viewer/gl.cpp)
set (server_srcs viewer/server.cpp)
set (viewer_srcs viewer/main.cpp)

add_executable (client ${client_srcs})
target_link_libraries (client ${Boost_SYSTEM_LIBRARY})
target_link_libraries (client glfw)
target_link_libraries (client ${OPENGL_gl_LIBRARY})

add_executable (server ${server_srcs})
target_link_libraries (server ${Boost_SYSTEM_LIBRARY})

add_executable (viewer ${viewer_srcs})
target_link_libraries (viewer glfw)
target_link_libraries (viewer ${OPENGL_gl_LIBRARY})

# Local Variables:
# indent-tabs-mode: nil
# End:
