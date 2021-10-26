#include "../include/YUYV422ReturningProcessor.hpp"

YUYV422ReturningProcessor::YUYV422ReturningProcessor(ISource &source, ITarget &target) :
    ReturningProcessor(source, target, GL_RGBA8UI, target.GetWidth() / 2)
{}

void YUYV422ReturningProcessor::DownloadFrame()
{
    glReadPixels(0, 0, OutputWidth / 2, OutputHeight, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, Output);
}
