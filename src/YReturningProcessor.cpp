#include "../include/YReturningProcessor.hpp"

YReturningProcessor::YReturningProcessor(ISource &source, ITarget &target) :
    ReturningProcessor(source, target, GL_R8UI, target.GetWidth())
{}

void YReturningProcessor::DownloadFrame()
{
    glReadPixels(0, 0, OutputWidth, OutputHeight, GL_RED_INTEGER, GL_UNSIGNED_BYTE, Output);
}
