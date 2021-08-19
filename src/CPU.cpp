#include "../include/CPU.hpp"

#include <stdlib.h>

CPU::CPU(IBuffer *sourceTarget) :
    Processor(sourceTarget)
{
    RandomizeLine();
}

CPU::~CPU() {}

void CPU::ProcessFrame()
{
    if (Frame >= 30 * 3)
        RandomizeLine(); // generujemy losowe parametry linii
    else
        ++Frame;
    DrawLine(); // rysujemy linię
}

void CPU::RandomizeLine()
{
    Y = rand() & 255;
    U = rand() & 255;
    V = rand() & 255;
    StartX = rand() % Width;
    StartY = rand() % Height;
    EndX = rand() % Width;
    EndY = rand() % Height;
    Frame = 0;
}

void CPU::DrawLine()
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

    dy1 = dy1 * Width;
    dy2 = dy2 * Width;

    int pixel = x + y * Width;
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
