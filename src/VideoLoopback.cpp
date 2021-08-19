#include "../include/VideoLoopback.hpp"

#include <stdexcept>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

VideoLoopback::VideoLoopback(const char *devicePath, int frameWidth, int frameHeight) :
    V4L2Device(devicePath, frameWidth, frameHeight, V4L2_CAP_VIDEO_OUTPUT, V4L2_BUF_TYPE_VIDEO_OUTPUT)
{}

VideoLoopback::~VideoLoopback() {}

uint8_t* VideoLoopback::GetBuffer() const
{
    return Buffer;
}

int VideoLoopback::GetWidth() const
{
    return Width;
}

int VideoLoopback::GetHeight() const
{
    return Height;
}

void VideoLoopback::UploadFrame()
{
    if (ioctl(FileDescriptor, VIDIOC_QBUF, &BufferInfo) < 0) // ustawiamy bufor wyjściowy w incoming queue urządzenia wyjściowego, aby następnie odczytało i wyświetliło z niego dane
        throw std::runtime_error(std::string("VideoLoopback::UploadFrame; VIDIOC_QBUF; ") + strerror(errno));
    if (ioctl(FileDescriptor, VIDIOC_DQBUF, &BufferInfo) < 0) // jeżeli urządzenie wyjściowe jeszcze nie odczytało danych z bufora, to proces na to czeka; jeżeli już odczytało, to zabieramy bufor z outgoing queue
        throw std::runtime_error(std::string("VideoLoopback::UploadFrame; VIDIOC_DQBUF; ") + strerror(errno));
}