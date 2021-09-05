#include "../include/Display.hpp"

#include <stdexcept>

Display::Display(const OpenGLContext &context) :
    OpenGLContext(context)
{}

Display::~Display() {}

uint8_t* Display::GetBuffer() const
{
    throw std::runtime_error("Display::GetBuffer; Display buffer is inaccessible.");
}

int Display::GetWidth() const
{
    return Width;
}

int Display::GetHeight() const
{
    return Height;
}

void Display::UploadFrame()
{
    glfwSwapBuffers(Window);
}
