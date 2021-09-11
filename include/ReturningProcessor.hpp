#ifndef ReturningProcessor_HPP
#define ReturningProcessor_HPP

#include "OutOfPlaceProcessor.hpp"
#include "ITarget.hpp"

class ReturningProcessor : public OutOfPlaceProcessor
{
private:
    uint8_t *const Output;
    unsigned int FBO, RBO;

public:
    ReturningProcessor(ISource *source, ITarget *target, const char *vertexShaderFilePath, const char *fragmentShaderFilePath);
    virtual ~ReturningProcessor();

    virtual void ProcessFrame() override;
    void DownloadFrame();
private:
    void CreateFramebuffer();
};

#endif // ReturningProcessor_HPP
