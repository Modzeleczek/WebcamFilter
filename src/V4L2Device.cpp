#include "../include/V4L2Device.hpp"

#include <fcntl.h>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

V4L2Device::V4L2Device(const char *path, int width, int height, int capability, v4l2_buf_type bufferType) :
    Width(width), Height(height)
{
    if ( (FileDescriptor = open(path, O_RDWR)) < 0 )
        throw std::runtime_error( std::string("V4L2Device::V4L2Device; ") + path + "; " + strerror(errno) );
    SetFormat(capability, bufferType);
    CreateBuffer(bufferType);
    Streaming = false;
}

V4L2Device::~V4L2Device()
{
    if (Streaming == true)
        StopStreaming();
    munmap(Buffer, BufferInfo.length);
    close(FileDescriptor);
}

void V4L2Device::SetFormat(int capability, v4l2_buf_type bufferType)
{
    v4l2_capability cap;
    if (ioctl(FileDescriptor, VIDIOC_QUERYCAP, &cap) < 0)
        throw std::runtime_error(std::string("V4L2Device::SetFormat; VIDIOC_QUERYCAP; ") + strerror(errno));
    if ((cap.capabilities & capability) == 0)
        throw std::runtime_error("V4L2Device::SetFormat; Device does not have required capability.");
    if ((cap.capabilities & V4L2_CAP_STREAMING) == 0)
        throw std::runtime_error("V4L2Device::SetFormat; Device does not handle streaming.");

    v4l2_format format;
    memset(&format, 0, sizeof(format));
    format.type = bufferType;
    if (ioctl(FileDescriptor, VIDIOC_G_FMT, &format) < 0)
        throw std::runtime_error(std::string("V4L2Device::SetFormat; VIDIOC_G_FMT; ") + strerror(errno));
    format.fmt.pix.field = V4L2_FIELD_NONE;
    format.fmt.pix.width = Width;
    format.fmt.pix.height = Height;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; // YUYV to to samo co YUV422
    if (ioctl(FileDescriptor, VIDIOC_S_FMT, &format) < 0)
        throw std::runtime_error(std::string("V4L2Device::SetFormat; VIDIOC_S_FMT; ") + strerror(errno));
}

void V4L2Device::CreateBuffer(v4l2_buf_type bufferType)
{
    v4l2_requestbuffers bufferRequest;
    memset(&bufferRequest, 0, sizeof(bufferRequest));
    bufferRequest.type = bufferType;
    bufferRequest.memory = V4L2_MEMORY_MMAP;
    bufferRequest.count = 1;
    if (ioctl(FileDescriptor, VIDIOC_REQBUFS, &bufferRequest) < 0)
        throw std::runtime_error(std::string("V4L2Device::CreateBuffer; VIDIOC_REQBUFS; ") + strerror(errno));

    BufferInfo.type = bufferType;
    BufferInfo.memory = V4L2_MEMORY_MMAP;
    BufferInfo.index = 0; // żądamy przygotowania bufora o indeksie 0
    BufferInfo.reserved = 0; // według dokumentacji (https://www.kernel.org/doc/html/v4.9/media/uapi/v4l/buffer.html) trzeba ustawić 0
    BufferInfo.reserved2 = 0; // też trzeba ustawić 0
    if (ioctl(FileDescriptor, VIDIOC_QUERYBUF, &BufferInfo) < 0)
        throw std::runtime_error(std::string("V4L2Device::CreateBuffer; VIDIOC_QUERYBUF; ") + strerror(errno));

    Buffer = (uint8_t*) mmap(NULL, BufferInfo.length, PROT_READ | PROT_WRITE, MAP_SHARED, FileDescriptor, BufferInfo.m.offset);
    if (Buffer == MAP_FAILED)
        throw std::runtime_error(std::string("V4L2Device::CreateBuffer; mmap; ") + strerror(errno));
    memset(Buffer, 0, BufferInfo.length);
}

void V4L2Device::StartStreaming()
{
    if (Streaming == true)
        throw std::runtime_error("V4L2Device::StartStreaming; Device is already streaming.");
    if (ioctl(FileDescriptor, VIDIOC_STREAMON, &BufferInfo.type) < 0)
        throw std::runtime_error("V4L2Device::StartStreaming; VIDIOC_STREAMON");
    Streaming = true;
}

void V4L2Device::StopStreaming()
{
    if (Streaming == false)
        throw std::runtime_error("V4L2Device::StopStreaming; Device is not streaming yet.");
    if (ioctl(FileDescriptor, VIDIOC_STREAMOFF, &BufferInfo.type) < 0)
        throw std::runtime_error("V4L2Device::StopStreaming; VIDIOC_STREAMOFF");
    Streaming = false;
}
