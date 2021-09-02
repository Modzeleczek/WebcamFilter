#ifndef SequentialPipeline_HPP
#define SequentialPipeline_HPP

#include "SequentialSink.hpp"

class SequentialPipeline : public SequentialSink
{
private:
    ITarget &Target; // przetwarzanie sekwencyjne z zapisem do bufora wyjściowego

public:
    SequentialPipeline(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target);
    SequentialPipeline(const SequentialPipeline &) = delete;
    virtual ~SequentialPipeline();

    virtual void Start() override;
};

#endif // SequentialPipeline_HPP
