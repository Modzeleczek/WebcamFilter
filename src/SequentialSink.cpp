#include "../include/SequentialSink.hpp"

SequentialSink::SequentialSink(ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target) :
    Runner(source, ipp, oopp, target)
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
        Target.UploadFrame();
    }
}
