#ifndef VideoLoopback_HPP
#define VideoLoopback_HPP

#include "ITarget.hpp"
#include "V4L2Device.hpp"

#include <linux/videodev2.h>

class VideoLoopback : public V4L2Device, public ITarget
{
public:
    VideoLoopback(const char *devicePath, int frameWidth, int frameHeight);
    ~VideoLoopback();

public:
    uint8_t* GetBuffer() const override;
    int GetWidth() const override;
    int GetHeight() const override;
    void UploadFrame() override;
};

#endif // VideoLoopback_HPP
