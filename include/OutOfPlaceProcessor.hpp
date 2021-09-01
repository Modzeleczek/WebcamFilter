#ifndef OutOfPlaceProcessor_HPP
#define OutOfPlaceProcessor_HPP

#include "IBuffer.hpp"
#include "Processor.hpp"

class OutOfPlaceProcessor : public Processor
{
private:
    uint8_t *const Output;
    unsigned int Program, VAO, VBO, EBO, Texture1, FBO, RBO;

public:
    OutOfPlaceProcessor(IBuffer *source, IBuffer *target, const char *vertexShaderFilePath, const char *fragmentShaderFilePath);
    virtual ~OutOfPlaceProcessor();

    void UploadFrame();
    virtual void ProcessFrame() override;
    void DownloadFrame();
private:
    void CheckShaderCompileStatus(unsigned int handle);
    void CheckProgramLinkStatus(unsigned int handle);
    void CreateTexture();
    void CreateProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
    void SetConstantUniforms();
    void CreateRectangle();
    void CreateFramebuffer();
};

#endif // OutOfPlaceProcessor_HPP
