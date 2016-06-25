set (viewer_srcs
  viewer/main.cpp
  )

add_executable (viewer ${viewer_srcs})
target_link_libraries (viewer glfw)
target_link_libraries (viewer ${OPENGL_gl_LIBRARY})

# Local Variables:
# indent-tabs-mode: nil
# End:
