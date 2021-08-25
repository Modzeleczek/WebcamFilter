#ifndef VideoLoopback_HPP
#define VideoLoopback_HPP

#include "ITarget.hpp"
#include "V4L2Device.hpp"

#include <linux/videodev2.h>

class VideoLoopback : public V4L2Device, public ITarget
{
public:
    VideoLoopback(const char *devicePath, int frameWidth, int frameHeight);
    virtual ~VideoLoopback();

public:
    virtual uint8_t* GetBuffer() const override;
    virtual int GetWidth() const override;
    virtual int GetHeight() const override;
    virtual void UploadFrame() override;
};

#endif // VideoLoopback_HPP
