#include "../include/GenericSource.hpp"

#include <stdexcept>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

GenericSource::GenericSource(int frameWidth, int frameHeight) :
    Width(frameWidth),
    Height(frameHeight),
    Buffer(new uint8_t[(frameWidth / 2) * frameHeight * 4]), // YUV422
    x(123456789),
    y(362436069),
    z(521288629)
{
    if (Buffer == NULL)
        throw std::runtime_error("GenericSource::GenericSource; Buffer was not allocated.");
}

GenericSource::~GenericSource()
{
    delete[] Buffer;
}

uint8_t* GenericSource::GetBuffer() const
{
    return Buffer;
}

int GenericSource::GetWidth() const
{
    return Width;
}

int GenericSource::GetHeight() const
{
    return Height;
}

unsigned long GenericSource::Xorshf96()
{
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;
    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;
    return z;
}

// void GenericSource::DownloadFrame()
// {
//     int i = 0;
//     while (i < (Width / 2) * Height * 4)
//     {
//         unsigned long r = Xorshf96(); // unsigned long ma 8 bajtów
//         // 1. para pikseli
//         Buffer[i++] = r; // >> (0 * 8); // Y1
//         Buffer[i++] = r >> (1 * 8); // U
//         Buffer[i++] = r >> (2 * 8); // Y2
//         Buffer[i++] = r >> (3 * 8); // V
//         // 2. para pikseli
//         Buffer[i++] = r >> (4 * 8); // Y1
//         Buffer[i++] = r >> (5 * 8); // U
//         Buffer[i++] = r >> (6 * 8); // Y2
//         Buffer[i++] = r >> (7 * 8); // V
//     }
// }

void GenericSource::DownloadFrame()
{
    unsigned long *b = (unsigned long*)Buffer;
    int i = 0;
    while (i < ((Width / 2) * Height) / 2)
        b[i++] = Xorshf96(); // unsigned long ma 8 bajtów, więc od razu ustawiamy 2 pary pikseli (każda ma 4 B), czyli 4 piksele
}
