#ifndef IBuffer_HPP
#define IBuffer_HPP

#include <stdint.h>

class IBuffer // interfejs
{
public:
    virtual ~IBuffer() {};

    virtual uint8_t* GetBuffer() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
};

#endif // IBuffer_HPP
