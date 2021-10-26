#ifndef YReturningProcessor_HPP
#define YReturningProcessor_HPP

#include "ReturningProcessor.hpp"

class YReturningProcessor : public ReturningProcessor
{
public:
    YReturningProcessor(ISource &source, ITarget &target);

    virtual void DownloadFrame() override;
};

#endif // YReturningProcessor_HPP
