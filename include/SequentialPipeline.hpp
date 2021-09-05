#ifndef SequentialPipeline_HPP
#define SequentialPipeline_HPP

#include "SequentialSink.hpp"
#include "ReturningProcessor.hpp"

class SequentialPipeline : public SequentialSink // przetwarzanie sekwencyjne z zapisem do bufora wyjściowego
{
public:
    SequentialPipeline(ISource &source, InPlaceProcessor &ipp, ReturningProcessor &rp, ITarget &target);
    SequentialPipeline(const SequentialPipeline &) = delete;
    virtual ~SequentialPipeline();

    virtual void Start() override;
};

#endif // SequentialPipeline_HPP
