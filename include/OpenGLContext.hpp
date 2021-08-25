#ifndef OpenGLContext_HPP
#define OpenGLContext_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class OpenGLContext
{
private:
    GLFWwindow *Window;

public:
    OpenGLContext(int width, int height);
    ~OpenGLContext();

    void UseOnCurrentThread();
};

#endif // OpenGLContext_HPP
