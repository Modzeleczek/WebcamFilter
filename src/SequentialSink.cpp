#include "../include/SequentialSink.hpp"

SequentialSink::SequentialSink(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp) :
    Runner(context, source, ipp, oopp)
{}

SequentialSink::~SequentialSink() {}

void SequentialSink::Start()
{
    Running = true;
    while (Running == true)
    {
        Source.DownloadFrame();
        IPP.ProcessFrame();
        OOPP.UploadFrame();
        OOPP.ProcessFrame();
    }
}
