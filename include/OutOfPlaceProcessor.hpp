#ifndef OutOfPlaceProcessor_HPP
#define OutOfPlaceProcessor_HPP

#include "Processor.hpp"
#include "ISource.hpp"

#include <initializer_list>

class OutOfPlaceProcessor : public Processor
{
protected:
    unsigned int Program, VAO, VBO, EBO, Texture1;

protected:
    struct Vertex
    {
        float Position[2], TextureCoord[2];
    };
    OutOfPlaceProcessor(ISource *src, const char *vsPath, const char *fsPath, const std::initializer_list<Vertex> &vert);
public:
    OutOfPlaceProcessor(ISource *source, const char *vertexShaderFilePath, const char *fragmentShaderFilePath);
    virtual ~OutOfPlaceProcessor();

    void UploadFrame();
    virtual void ProcessFrame() override;
protected:
    void CheckShaderCompileStatus(unsigned int handle);
    void CheckProgramLinkStatus(unsigned int handle);
    void CreateTexture();
    void CreateProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
    void SetConstantUniforms();
    void CreateRectangle(const std::initializer_list<Vertex> &vert);
};

#endif // OutOfPlaceProcessor_HPP
