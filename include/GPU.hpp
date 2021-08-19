#ifndef GPU_HPP
#define GPU_HPP

#include "IBuffer.hpp"
#include "Processor.hpp"

class GPU : public Processor
{
private:
    const uint8_t *Output;
    unsigned int Program, VAO, VBO, EBO, Texture1, FBO, RBO;

public:
    GPU(IBuffer *source, IBuffer *target, const char *vertexShaderFilePath, const char *fragmentShaderFilePath);
    ~GPU();

    void ProcessFrame() override;
private:
    void CheckShaderCompileStatus(unsigned int handle);
    void CheckProgramLinkStatus(unsigned int handle);
    void CreateTexture();
    void CreateProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
    void SetConstantUniforms();
    void CreateRectangle();
    void CreateFramebuffer();
};

#endif // GPU_HPP