#ifndef ITarget_HPP
#define ITarget_HPP

#include "IBuffer.hpp"

class ITarget : public IBuffer // interfejs
{
public:
    virtual ~ITarget() {};

    virtual void UploadFrame() = 0;
};

#endif // ITarget_HPP
