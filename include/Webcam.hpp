#ifndef Webcam_HPP
#define Webcam_HPP

#include "ISource.hpp"
#include "V4L2Device.hpp"

#include <linux/videodev2.h>

class Webcam : public V4L2Device, public ISource
{
public:
    Webcam(const char *devicePath, int frameWidth, int frameHeight);
    ~Webcam();

public:
    uint8_t* GetBuffer() const override;
    int GetWidth() const override;
    int GetHeight() const override;
    void DownloadFrame() override;
};

#endif // Webcam
