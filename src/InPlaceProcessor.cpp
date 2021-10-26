#include "../include/InPlaceProcessor.hpp"

#include <stdlib.h>

InPlaceProcessor::InPlaceProcessor(IBuffer &sourceTarget) :
    Processor(sourceTarget)
{
    RandomizeLine();
}

InPlaceProcessor::~InPlaceProcessor() {}

void InPlaceProcessor::ProcessFrame()
{
    if (Frame >= 30 * 3)
        RandomizeLine(); // generujemy losowe parametry linii
    else
        ++Frame;
    DrawLine(); // rysujemy linię
}

void InPlaceProcessor::RandomizeLine()
{
    Y = rand() & 255;
    U = rand() & 255;
    V = rand() & 255;
    StartX = rand() % InputWidth;
    StartY = rand() % InputHeight;
    EndX = rand() % InputWidth;
    EndY = rand() % InputHeight;
    Frame = 0;
}

void InPlaceProcessor::DrawLine()
{
    int x = StartX, y = StartY;
    int w = EndX - x;
    int h = EndY - y;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
    if (w < 0)
        dx1 = dx2 = -1;
    else if (w > 0)
        dx1 = dx2 = 1;
    if (h < 0)
        dy1 = -1;
    else if (h > 0)
        dy1 = 1;
    int longest = abs(w);
    int shortest = abs(h);
    if (longest <= shortest)
    {
        int temp = longest;
        longest = shortest;
        shortest = temp;
        if (h < 0)
            dy2 = -1;
        else if (h > 0)
            dy2 = 1;
        dx2 = 0;
    }
    int numerator = longest >> 1;

    dy1 = dy1 * InputWidth;
    dy2 = dy2 * InputWidth;

    int pixel = x + y * InputWidth;
    int i;
    for (i = 0; i <= longest; i++)
    {
        uint8_t *pPixelPair = (uint8_t*)Input + ((pixel / 2) * 4); // (pixel >> 1) << 2;
        uint8_t *pU = pPixelPair + 1;
        uint8_t *pV = pPixelPair + 3;
        uint8_t *pY = pPixelPair + (pixel % 2); // pixel & 1
        *pY = Y;
        *pU = U;
        *pV = V;
        numerator += shortest;
        if (!(numerator < longest))
        {
            numerator -= longest;
            pixel += dx1 + dy1;
        }
        else
            pixel += dx2 + dy2;
    }
}
