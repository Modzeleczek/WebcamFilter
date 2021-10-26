#ifndef OutOfPlaceProcessor_HPP
#define OutOfPlaceProcessor_HPP

#include "Processor.hpp"
#include "ISource.hpp"
#include "ShaderProgram.hpp"

#include <initializer_list>

class OutOfPlaceProcessor : public Processor
{
protected:
    GLuint Program, VAO, VBO, EBO, Texture1;

protected:
    struct Vertex
    {
        float Position[2], TextureCoord[2];
    };
    OutOfPlaceProcessor(ISource &src, const std::initializer_list<Vertex> &vert);
public:
    OutOfPlaceProcessor(ISource &source);
    virtual ~OutOfPlaceProcessor();

    void UploadFrame();
    virtual void ProcessFrame() override;
protected:
    void CreateTexture();
    void CreateRectangle(const std::initializer_list<Vertex> &vert);
};

#endif // OutOfPlaceProcessor_HPP
