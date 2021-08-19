#include "../include/Processor.hpp"

#include <stdexcept>

Processor::Processor(IBuffer *source) :
    Input(source->GetBuffer()),
    Width(source->GetWidth()),
    Height(source->GetHeight())
{}

Processor::~Processor() {}
