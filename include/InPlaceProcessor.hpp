#ifndef InPlaceProcessor_HPP
#define InPlaceProcessor_HPP

#include "IBuffer.hpp"
#include "Processor.hpp"

class InPlaceProcessor : public Processor
{
private:
    uint32_t Frame;
    uint8_t Y, U, V;
    int StartX, StartY, EndX, EndY;

public:
    InPlaceProcessor(IBuffer &buffer); // przetwarzamy klatkę w miejscu, czyli bez kopiowania z bufora wejściowego do wyjściowego
    virtual ~InPlaceProcessor();

    virtual void ProcessFrame() override;
private:
    void RandomizeLine();
    void DrawLine();
};

#endif // InPlaceProcessor_HPP
