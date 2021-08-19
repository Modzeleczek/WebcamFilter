#include "../include/Webcam.hpp"

#include <stdexcept>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

Webcam::Webcam(const char *devicePath, int frameWidth, int frameHeight) :
    V4L2Device(devicePath, frameWidth, frameHeight, V4L2_CAP_VIDEO_CAPTURE, V4L2_BUF_TYPE_VIDEO_CAPTURE)
{}

Webcam::~Webcam() {} // https://stackoverflow.com/questions/654428/what-is-the-order-in-which-the-destructors-and-the-constructors-are-called-in-c; wirtualne destruktory są wykonywane w kolejności odwrotnej do konstruktorów, czyli od klasy najbardziej pochodnej do najbardziej bazowej, więc ~V4L2Device() jest wykonywane automatycznie po tym destruktorze (~Webcam())

uint8_t* Webcam::GetBuffer() const
{
    return Buffer;
}

int Webcam::GetWidth() const
{
    return Width;
}

int Webcam::GetHeight() const
{
    return Height;
}

void Webcam::DownloadFrame()
{
    if (ioctl(FileDescriptor, VIDIOC_QBUF, &BufferInfo) < 0) // ustawiamy bufor wejściowy w incoming queue urządzenia wejściowego, aby następnie zapisało do niego dane
        throw std::runtime_error(std::string("Webcam::DownloadFrame; VIDIOC_QBUF; ") + strerror(errno));
    if (ioctl(FileDescriptor, VIDIOC_DQBUF, &BufferInfo) < 0) // jeżeli urządzenie wejściowe jeszcze nie zapisało danych do bufora, to proces na to czeka; jeżeli już zapisało, to zabieramy bufor z outgoing queue
        throw std::runtime_error(std::string("Webcam::DownloadFrame; VIDIOC_DQBUF; ") + strerror(errno));
}
