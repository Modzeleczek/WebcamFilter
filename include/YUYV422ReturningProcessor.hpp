#ifndef YUYV422ReturningProcessor_HPP
#define YUYV422ReturningProcessor_HPP

#include "ReturningProcessor.hpp"

class YUYV422ReturningProcessor : public ReturningProcessor
{
public:
    YUYV422ReturningProcessor(ISource &source, ITarget &target);

    virtual void DownloadFrame() override;
};

#endif // YUYV422ReturningProcessor_HPP
