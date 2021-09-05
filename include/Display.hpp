#ifndef Display_HPP
#define Display_HPP

#include "OpenGLContext.hpp"
#include "ITarget.hpp"

class Display : public OpenGLContext, public ITarget
{
public:
    Display(const OpenGLContext &context);
    virtual ~Display();

    virtual uint8_t* GetBuffer() const override;
    virtual int GetWidth() const override;
    virtual int GetHeight() const override;
    virtual void UploadFrame() override;
};

#endif // Display_HPP
