#include "../include/ReturningProcessor.hpp"

#include <GL/glew.h>
#include <stdexcept>

ReturningProcessor::ReturningProcessor(ISource &source, ITarget &target, GLenum RBOInternalFormat, GLsizei RBOWidth) :
    OutOfPlaceProcessor(source,
        // pozycje wierzchołków prostokąta; tekstura pokrywa się z prostokątem
        {
            // pozycja wierzchołka w przestrzeni kamery (od -1 do 1)
                        // który punkt tekstury powinien znajdować się w danym wierzchołku (od 0 do 1)
            {1.f, 1.f,  1.f, 1.f}, // top right
            {1.f, -1.f,  1.f, 0.f}, // bottom right
            {-1.f, -1.f,  0.f, 0.f}, // bottom left
            {-1.f, 1.f,  0.f, 1.f} // top left
        }),
    Output(target.GetBuffer()),
    OutputWidth(target.GetWidth()),
    OutputHeight(target.GetHeight())
{
    CreateFramebuffer(RBOInternalFormat, RBOWidth);
    // bindujemy FBO, aby renderować do niego scenę (składającą się tylko z prostokąta)
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

ReturningProcessor::~ReturningProcessor()
{
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &FBO);
}

void ReturningProcessor::CreateFramebuffer(GLenum RBOInternalFormat, GLsizei RBOWidth) // tworzymy Framebuffer Object (FBO) i Render Buffer Object (RBO) jako color attachment do FBO
{
    // tworzymy uchwyt do FBO
    glGenFramebuffers(1, &FBO);
    // bindujemy FBO zamiast domyślnego Framebuffer Objectu, który jest tworzony wraz z oknem programu i do którego GPU domyślnie renderuje
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // tworzymy uchwyt do RBO
    glGenRenderbuffers(1, &RBO);
    // bindujemy RBO
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    // rezerwujemy pamięć RBO
    glRenderbufferStorage(GL_RENDERBUFFER, RBOInternalFormat, RBOWidth, OutputHeight);
    // odbindowujemy RBO, bo zbindowaliśmy uchwyt do niego tylko po to, aby zarezerwować na niego pamięć
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // dołączamy RBO jako color attachment do FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);

    // sprawdzamy, czy FBO jest kompletne
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("ReturningProcessor::CreateFramebuffer; Framebuffer is not complete.");
    // odbindowujemy FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
