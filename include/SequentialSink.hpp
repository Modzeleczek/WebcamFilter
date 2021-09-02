#ifndef SequentialSink_HPP
#define SequentialSink_HPP

#include "Runner.hpp"

class SequentialSink : public Runner // przetwarzanie sekwencyjne bez bufora wyjściowego
{
public:
    SequentialSink(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp);
    SequentialSink(const SequentialSink &) = delete;
    virtual ~SequentialSink();

    virtual void Start() override;
};

#endif // SequentialSink_HPP
