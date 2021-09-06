#ifndef OpenGLContext_HPP
#define OpenGLContext_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class OpenGLContext
{
protected:
    GLFWwindow *Window;
    const int Width, Height;

public:
    OpenGLContext(int width, int height);
    OpenGLContext(const OpenGLContext &object);
    virtual ~OpenGLContext();

    void UseOnCurrentThread();
    void PrintGPUInfo();
};

#endif // OpenGLContext_HPP
