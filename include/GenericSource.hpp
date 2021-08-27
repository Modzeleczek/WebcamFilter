#ifndef GenericSource_HPP
#define GenericSource_HPP

#include "ISource.hpp"
#include "V4L2Device.hpp"

#include <linux/videodev2.h>

class GenericSource : public ISource
{
private:
    const int Width, Height;
    uint8_t *Buffer;
    unsigned long x, y, z;

public:
    GenericSource(int frameWidth, int frameHeight);
    virtual ~GenericSource();

public:
    virtual uint8_t* GetBuffer() const override;
    virtual int GetWidth() const override;
    virtual int GetHeight() const override;
    virtual void DownloadFrame() override;
private:
    unsigned long Xorshf96();
};

#endif // GenericSource_HPP
