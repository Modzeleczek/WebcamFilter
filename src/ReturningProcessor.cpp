#include "../include/ReturningProcessor.hpp"

#include <GL/glew.h>
#include <stdexcept>

ReturningProcessor::ReturningProcessor(IBuffer *source, IBuffer *target, const char *vertexShaderFilePath, const char *fragmentShaderFilePath) :
    OutOfPlaceProcessor(source, vertexShaderFilePath, fragmentShaderFilePath,
        // pozycje wierzchołków prostokąta; tekstura pokrywa się z prostokątem
        {
            // pozycja wierzchołka w przestrzeni kamery (od -1 do 1)
                        // który punkt tekstury powinien znajdować się w danym wierzchołku (od 0 do 1)
            {1.f, 1.f,  1.f, 1.f}, // top right
            {1.f, -1.f,  1.f, 0.f}, // bottom right
            {-1.f, -1.f,  0.f, 0.f}, // bottom left
            {-1.f, 1.f,  0.f, 1.f} // top left
        }),
    Output(target->GetBuffer())
{
    if (Width != target->GetWidth())
        throw std::runtime_error("ReturningProcessor::ReturningProcessor; Source and target have different width.");
    if (Height != target->GetHeight())
        throw std::runtime_error("ReturningProcessor::ReturningProcessor; Source and target have different height.");
    CreateFramebuffer();
    // bindujemy FBO, aby renderować do niego scenę (składającą się tylko z prostokąta)
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

ReturningProcessor::~ReturningProcessor()
{
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &FBO);
}

void ReturningProcessor::ProcessFrame()
{
    // aktywujemy jednostkę teksturującą (texture unit) o indeksie 0; nie trzeba tego robić, bo texture unit o indeksie 0 jest zawsze domyślnie aktywny; jeżeli używalibyśmy kilku tekstur jednocześnie, to musielibyśmy kolejno aktywować texture unit o indeksie n i zbindować do niego wybraną teksturę, a następnie to samo zrobić dla texture unita o indeksie n+1
    // glActiveTexture(GL_TEXTURE0);
    // bindujemy teksturę texture1 do aktualnie aktywnego texture unita
    // glBindTexture(GL_TEXTURE_2D, Texture1);

    // rysujemy prostokąt, używając jako typu prymitywu GL_TRIANGLES
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // nie trzeba za każdym razem odbindowywać VAO, bo używamy tylko jednego VAO
    // glBindVertexArray(0);
}

void ReturningProcessor::DownloadFrame()
{
    glReadPixels(0, 0, Width / 2, Height, GL_RGBA, GL_UNSIGNED_BYTE, Output);
}

void ReturningProcessor::CreateFramebuffer() // tworzymy Framebuffer Object (FBO) i Render Buffer Object (RBO) jako color attachment do FBO
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
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, Width / 2, Height);
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
