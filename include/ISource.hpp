#ifndef ISource_HPP
#define ISource_HPP

#include "IBuffer.hpp"

class ISource : public IBuffer // interfejs
{
public:
    virtual ~ISource() {};

    virtual void DownloadFrame() = 0;
};

#endif // ISource_HPP
