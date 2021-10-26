#ifndef ReturningProcessor_HPP
#define ReturningProcessor_HPP

#include "OutOfPlaceProcessor.hpp"
#include "ITarget.hpp"

class ReturningProcessor : public OutOfPlaceProcessor
{
protected:
    uint8_t *const Output;
    GLuint FBO, RBO;
    int OutputWidth, OutputHeight;

    ReturningProcessor(ISource &source, ITarget &target, GLenum RBOInternalFormat, GLsizei RBOWidth);
public:
    virtual ~ReturningProcessor();

    virtual void DownloadFrame() = 0;
protected:
    void CreateFramebuffer(GLenum RBOInternalFormat, GLsizei RBOWidth);
};

#endif // ReturningProcessor_HPP
