#ifndef SequentialSink_HPP
#define SequentialSink_HPP

#include "Runner.hpp"

class SequentialSink : public Runner // przetwarzanie sekwencyjne bez bufora wyjściowego, do którego byśmy jawnie zapisywali w niniejszym programie
{
public:
    SequentialSink(ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target);
    SequentialSink(const SequentialSink &) = delete;
    virtual ~SequentialSink();

    virtual void Start() override;
};

#endif // SequentialSink_HPP
