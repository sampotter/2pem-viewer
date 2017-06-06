#ifndef __VIEWER_GLHEADERS_HPP__
#define __VIEWER_GLHEADERS_HPP__

#if _WIN64
#    include <windows.h>
#endif

#if __linux__ || _WIN64
#    include <GL/glew.h>
#    include <GL/glu.h>
#endif

#include <GLFW/glfw3.h>

#endif // __VIEWER_GLHEADERS_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
