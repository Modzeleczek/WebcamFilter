#ifndef Webcam_HPP
#define Webcam_HPP

#include "ISource.hpp"
#include "V4L2Device.hpp"

#include <linux/videodev2.h>

class Webcam : public V4L2Device, public ISource
{
public:
    Webcam(const char *devicePath, int frameWidth, int frameHeight);
    virtual ~Webcam();

public:
    virtual uint8_t* GetBuffer() const override;
    virtual int GetWidth() const override;
    virtual int GetHeight() const override;
    virtual void DownloadFrame() override;
};

#endif // Webcam_HPP
