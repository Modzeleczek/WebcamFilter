#include "../include/Processor.hpp"

#include <stdexcept>

Processor::Processor(IBuffer &source) :
    Input(source.GetBuffer()),
    InputWidth(source.GetWidth()),
    InputHeight(source.GetHeight())
{}

Processor::~Processor() {}
