#ifndef CPU_HPP
#define CPU_HPP

#include "IBuffer.hpp"
#include "Processor.hpp"

class CPU : public Processor
{
private:
    uint32_t Frame;
    uint8_t Y, U, V;
    int StartX, StartY, EndX, EndY;

public:
    CPU(IBuffer *buffer); // przetwarzamy klatkę w miejscu, czyli bez kopiowania z bufora wejściowego do wyjściowego
    virtual ~CPU();

    virtual void ProcessFrame() override;
private:
    void RandomizeLine();
    void DrawLine();
};

#endif // CPU_HPP
