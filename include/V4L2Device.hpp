#ifndef V4L2Device_HPP
#define V4L2Device_HPP

#include <linux/videodev2.h>
#include <stdint.h>

class V4L2Device
{
protected:
    const int FileDescriptor;
    v4l2_buffer BufferInfo;
    const int Width, Height;
    uint8_t *Buffer;
    bool Streaming;

    V4L2Device(const char *path, int width, int height, int capability, v4l2_buf_type bufferType);
    virtual ~V4L2Device();

private:
    void SetFormat(int capability, v4l2_buf_type bufferType);
    void CreateBuffer(v4l2_buf_type bufferType);
public:
    void StartStreaming();
    void StopStreaming();
};

#endif // V4L2Device_HPP
