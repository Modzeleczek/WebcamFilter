#include "../include/OpenGLContext.hpp"

#include <stdexcept>
#include <GL/glew.h>

OpenGLContext::OpenGLContext(int width, int height) :
    Width(width),
    Height(height)
{
    if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error("OpenGLContext::OpenGLContext; glfwInit");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // tworzymy okno o szerokości WIDTH i wysokości HEIGHT; musi być stworzone dokładnie takie okno; jeżeli stworzymy za małe okno, to GPU nie wyrenderuje całej klatki; glfwCreateWindow oprócz okna tworzy też kontekst OpenGLa, który jest ustawiany w innych wątkach za pomocą glfwMakeContextCurrent
    Window = glfwCreateWindow(width, height, "ESC, close or SIGINT to quit", NULL, NULL);
    if (Window == NULL)
        throw std::runtime_error("OpenGLContext::OpenGLContext; glfwCreateWindow");
}

OpenGLContext::OpenGLContext(const OpenGLContext &object) :
    Window(object.Window),
    Width(object.Width),
    Height(object.Height)
{}

OpenGLContext::~OpenGLContext()
{
    glfwWindowShouldClose(Window);
    glfwDestroyWindow(Window);
    glfwTerminate();
}

void OpenGLContext::UseOnCurrentThread()
{
    if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error("OpenGLContext::UseOnCurrentThread; glfwInit");
    glfwMakeContextCurrent(Window);
    GLenum ret = glewInit();
    if (ret != GLEW_OK)
        throw std::runtime_error(std::string("OpenGLContext::UseOnCurrentThread; glewInit; ") + (const char*)glewGetErrorString(ret));
    if (glewIsSupported("GL_VERSION_3_3") == GL_FALSE)
        throw std::runtime_error("OpenGLContext::UseOnCurrentThread; OpenGL 3.3 unsupported");
}

void OpenGLContext::PrintGPUInfo()
{
    const GLubyte *vendor = glGetString(GL_VENDOR); // producent GPU
    const GLubyte *renderer = glGetString(GL_RENDERER); // model GPU
    printf("%s; %s", vendor, renderer);
}
